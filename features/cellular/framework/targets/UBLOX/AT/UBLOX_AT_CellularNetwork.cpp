/*
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "UBLOX_AT_CellularNetwork.h"
#include "UBLOX_AT_CellularStack.h"

using namespace mbed;

UBLOX_AT_CellularNetwork::UBLOX_AT_CellularNetwork(ATHandler &atHandler) : AT_CellularNetwork(atHandler)
{
	_op_act = operator_t::RAT_UTRAN;
    // The authentication to use
    _auth = NSAPI_SECURITY_UNKNOWN;
}

UBLOX_AT_CellularNetwork::~UBLOX_AT_CellularNetwork()
{
	disconnect_modem_stack();
}

NetworkStack *UBLOX_AT_CellularNetwork::get_stack()
{
    if (!_stack) {
        _stack = new UBLOX_AT_CellularStack(_at, _cid, _ip_stack_type);
    }
    return _stack;
}

bool UBLOX_AT_CellularNetwork::get_modem_stack_type(nsapi_ip_stack_t requested_stack)
{
    return requested_stack == IPV4_STACK ? true : false;
}

bool UBLOX_AT_CellularNetwork::has_registration(RegistrationType reg_type)
{
    return (reg_type == C_REG || reg_type == C_GREG);
}

nsapi_error_t UBLOX_AT_CellularNetwork::set_access_technology_impl(operator_t::RadioAccessTechnology opRat)
{
    switch(opRat) {
    case operator_t::RAT_UTRAN:
    case operator_t::RAT_EGPRS:
    case operator_t::RAT_E_UTRAN:
    case operator_t::RAT_CATM1:
    case operator_t::RAT_NB1: {
    	// Do nothing
    }
    break;
    default: {
        //TODO: Set as unknown or force to NB1?
        _op_act = operator_t::RAT_UNKNOWN;
        return NSAPI_ERROR_UNSUPPORTED;
    }
    break;
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t UBLOX_AT_CellularNetwork::connect()
{
    _at.lock();
    nsapi_error_t err = NSAPI_ERROR_NO_CONNECTION;

    err = open_data_channel();
    if (err != NSAPI_ERROR_OK) {
        // If new PSD context was created and failed to activate, delete it
        if (_new_context_set) {
            disconnect_modem_stack();
        }
        _at.unlock();

        _connect_status = NSAPI_STATUS_DISCONNECTED;
        if (_connection_status_cb) {
            _connection_status_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, NSAPI_STATUS_DISCONNECTED);
        }

        return err;
    }
    _at.unlock();

    _connect_status = NSAPI_STATUS_GLOBAL_UP;
    if (_connection_status_cb) {
        _connection_status_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, NSAPI_STATUS_GLOBAL_UP);
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t UBLOX_AT_CellularNetwork::open_data_channel()
{
    bool success = false;
    int active = 0;
    const char * config = NULL;
    //old way: _at.send("ATD*99***%d#", _cid) && _at.recv("CONNECT");
    nsapi_error_t err = NSAPI_ERROR_NO_CONNECTION;

    // do check for stack to validate that we have support for stack
    _stack = get_stack();
    if (!_stack) {
        return err;
    }
	
    _at.cmd_start("AT+UPSND=" PROFILE ",8");
    _at.cmd_stop();
    _at.resp_start("+UPSND:");
    _at.read_int();
    _at.read_int();
    active = _at.read_int();
    _at.resp_stop();

    if (active == 0) {
        // Attempt to connect
        do {
            success = activate_profile(_apn, _uname, _pwd);
        } while (!success && config && *config);
    } else {
        // If the profile is already active, we're good
        success = true;
    }
	
    err = (_at.get_last_error() == NSAPI_ERROR_OK) ? NSAPI_ERROR_OK : NSAPI_ERROR_NO_CONNECTION;

    return err;
}

bool UBLOX_AT_CellularNetwork::activate_profile(const char* apn,
                                                const char* username,
                                                const char* password)
{
    bool activated = false;
    bool success = false;

    // Set up the APN
    if (*apn) {
        _at.cmd_start("AT+UPSD=0,1,");
        _at.write_string(apn);
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();

        if(_at.get_last_error()) {
            success = false;
        } else {
            success = true;
        }
    }
	
    if (success && *username) {
        _at.cmd_start("AT+UPSD=" PROFILE ",2,");
        _at.write_string(username);
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();

        if (_at.get_last_error()) {
            success = false;
        } else {
            success = true;
        }
    }
	
    if (success && *password) {
        _at.cmd_start("AT+UPSD=" PROFILE ",3,");
        _at.write_string(password);
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();

        if(_at.get_last_error()) {
            success = false;
        } else {
            success = true;
        }
    }

    if (success) {
        _at.cmd_start("AT+UPSD=" PROFILE ",7,\"0.0.0.0\"");
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();

        _auth = NSAPI_SECURITY_NONE;

        // Set up the authentication protocol
        // 0 = none
        // 1 = PAP (Password Authentication Protocol)
        // 2 = CHAP (Challenge Handshake Authentication Protocol)
        for (int protocol = nsapi_security_to_modem_security(NSAPI_SECURITY_NONE);
             success && (protocol <= nsapi_security_to_modem_security(NSAPI_SECURITY_CHAP)); protocol++) {
            if ((_auth == NSAPI_SECURITY_UNKNOWN) || (nsapi_security_to_modem_security(_auth) == protocol)) {
                _at.cmd_start("AT+UPSD=0,6,");
                _at.write_int(protocol);
                _at.cmd_stop();
                _at.resp_start();
                _at.resp_stop();

                _at.cmd_start("AT+UPSDA=0,3");
                _at.cmd_stop();
                _at.resp_start();
                _at.resp_stop();

                activated = 1;
            }
        }
    }

    return activated;
}

// Convert nsapi_security_t to the modem security numbers
int UBLOX_AT_CellularNetwork::nsapi_security_to_modem_security(nsapi_security_t nsapi_security)
{
    int modem_security = 3;

    switch (nsapi_security)
    {
        case NSAPI_SECURITY_NONE:
            modem_security = 0;
            break;
        case NSAPI_SECURITY_PAP:
            modem_security = 1;
            break;
        case NSAPI_SECURITY_CHAP:
            modem_security = 2;
            break;
        case NSAPI_SECURITY_UNKNOWN:
            modem_security = 3;
            break;
        default:
            modem_security = 3;
            break;
    }

    return modem_security;
}

// Disconnect the on board IP stack of the modem.
bool UBLOX_AT_CellularNetwork::disconnect_modem_stack()
{
    bool success = false;

    if (get_ip_address() != NULL) {
        _at.cmd_start("AT+UPSDA=" PROFILE ",4");
        _at.cmd_stop();

        _at.resp_start("OK");
        if (_at.info_resp()) {
            success = true;
        }
        _at.resp_stop();
    }
	
    return success;
}