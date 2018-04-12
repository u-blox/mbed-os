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
}

UBLOX_AT_CellularNetwork::~UBLOX_AT_CellularNetwork()
{

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
