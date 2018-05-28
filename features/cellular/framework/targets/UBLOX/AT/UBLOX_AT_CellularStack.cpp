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

#include "UBLOX_AT_CellularStack.h"
#include "CellularUtil.h"
#include "mbed_wait_api.h"

using namespace mbed;
using namespace mbed_cellular_util;

UBLOX_AT_CellularStack::UBLOX_AT_CellularStack(ATHandler &atHandler, int cid, nsapi_ip_stack_t stack_type) : AT_CellularStack(atHandler, cid, stack_type)
{
    // URC handlers for sockets
    _at.set_urc_handler("+UUSORD:", callback(this, &UBLOX_AT_CellularStack::UUSORD_URC));
    _at.set_urc_handler("+UUSORF:", callback(this, &UBLOX_AT_CellularStack::UUSORF_URC));
    _at.set_urc_handler("+UUSOCL:", callback(this, &UBLOX_AT_CellularStack::UUSOCL_URC));
    _at.set_urc_handler("+UUPSDD:", callback(this, &UBLOX_AT_CellularStack::UUPSDD_URC));
}

UBLOX_AT_CellularStack::~UBLOX_AT_CellularStack()
{
}

nsapi_error_t UBLOX_AT_CellularStack::socket_listen(nsapi_socket_t handle, int backlog)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t UBLOX_AT_CellularStack::socket_accept(void *server, void **socket, SocketAddress *addr)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

// Callback for Socket Read URC.
void UBLOX_AT_CellularStack::UUSORD_URC()
{
    int a,b;
    char buf[32];
    CellularSocket *socket;

    // +UUSORD: <socket>,<length>
    if (_at.read_string(buf, sizeof (buf), '\n') > 0) {
        if (sscanf(buf, ": %d,%d", &a, &b) == 2) {
            socket = find_socket(a);
            if (socket != NULL) {
                socket->rx_avail = true;
                // No debug prints here as they can affect timing
                // and cause data loss in UARTSerial
                if (socket->_cb != NULL) {
                    socket->_cb(socket->_data);
                }
            }
        }
    }
}

// Callback for Socket Read From URC.
void UBLOX_AT_CellularStack::UUSORF_URC()
{
    int a,b;
    char buf[32];
    CellularSocket *socket;

    // +UUSORF: <socket>,<length>
    if (_at.read_string(buf, sizeof (buf), '\n') > 0) {
        if (sscanf(buf, ": %d,%d", &a, &b) == 2) {
            socket = find_socket(a);
            if (socket != NULL) {
                socket->rx_avail = true;
                // No debug prints here as they can affect timing
                // and cause data loss in UARTSerial
                if (socket->_cb != NULL) {
                    socket->_cb(socket->_data);
                }
            }
        }
    }
}

// Callback for Socket Close URC.
void UBLOX_AT_CellularStack::UUSOCL_URC()
{
    int a;
    char buf[32];
    CellularSocket *socket;

    // +UUSOCL: <socket>
    if (_at.read_string(buf, sizeof (buf), '\n') > 0) {
        if (sscanf(buf, ": %d", &a) == 1) {
            socket = find_socket(a);
            clear_socket(socket);
        }
    }
}

// Callback for UUPSDD.
void UBLOX_AT_CellularStack::UUPSDD_URC()
{
    int a;
    char buf[32];
    CellularSocket *socket;

    // +UUPSDD: <socket>
    if (_at.read_string(buf, sizeof (buf), '\n') > 0) {
        if (sscanf(buf, ": %d", &a) == 1) {
            socket = find_socket(a);
            clear_socket(socket);
        }
    }
}


int UBLOX_AT_CellularStack::get_max_socket_count()
{
    return UBLOX_U201_SOCKET_MAX;
}

int UBLOX_AT_CellularStack::get_max_packet_size()
{
    return UBLOX_U201_MAX_PACKET_SIZE;
}

bool UBLOX_AT_CellularStack::is_protocol_supported(nsapi_protocol_t protocol)
{
    return (protocol == NSAPI_UDP || protocol == NSAPI_TCP);
}

nsapi_error_t UBLOX_AT_CellularStack::socket_open(nsapi_socket_t *handle, nsapi_protocol_t proto)
{
    if (!is_protocol_supported(proto) || !handle) {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    int max_socket_count = get_max_socket_count();

    if (!_socket) {
        _socket = new CellularSocket*[max_socket_count];
        if (!_socket) {
            return NSAPI_ERROR_NO_SOCKET;
        }
        _socket_count = max_socket_count;
        for (int i = 0; i < max_socket_count; i++) {
            _socket[i] = 0;
        }
    }

    int index = -1;
    for (int i = 0; i < max_socket_count; i++) {
        if (!_socket[i]) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    // create local socket structure, socket on modem is created when app calls sendto/recvfrom
    _socket[index] = new CellularSocket;
    CellularSocket *psock;
    psock = _socket[index];
    memset(psock, 0, sizeof(CellularSocket));
    SocketAddress addr(0, get_dynamic_ip_port());
    psock->id = index;
    psock->localAddress = addr;
    psock->proto = proto;
    nsapi_error_t err = create_socket_impl(psock);

    if (err == NSAPI_ERROR_OK) {
        *handle = psock;
    }

    return err;
}

nsapi_error_t UBLOX_AT_CellularStack::create_socket_impl(CellularSocket *socket)
{
    int sock_id;
    bool socketOpenWorking = false;

    _at.lock();
    if (socket->proto == NSAPI_UDP) {
        _at.cmd_start("AT+USOCR=17");
        _at.cmd_stop();

        _at.resp_start("+USOCR:");
        sock_id = _at.read_int();
        _at.resp_stop();
    } else if(socket->proto == NSAPI_TCP) {
        _at.cmd_start("AT+USOCR=6");
        _at.cmd_stop();

        _at.resp_start("+USOCR:");
        sock_id = _at.read_int();
        _at.resp_stop();
    } // Unsupported protocol is checked in "is_protocol_supported" function 
    _at.unlock();

    socketOpenWorking = (_at.get_last_error() == NSAPI_ERROR_OK);
    if (!socketOpenWorking) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    // Check for duplicate socket id delivered by modem
    for (int i = 0; i < UBLOX_U201_SOCKET_MAX; i++) {
        CellularSocket *sock = _socket[i];
        if (sock && sock->created && sock->id == sock_id) {
            return NSAPI_ERROR_NO_SOCKET;
        }
    }

    socket->id = sock_id;
    socket->created = true;

    return NSAPI_ERROR_OK;
}

nsapi_error_t UBLOX_AT_CellularStack::socket_connect(nsapi_socket_t handle, const SocketAddress &addr)
{
    CellularSocket *socket = (CellularSocket *)handle;

    if (!socket) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    _at.lock();
    _at.cmd_start("AT+USOCO=");
    _at.write_int(socket->id);
    _at.write_string(addr.get_ip_address(), true);
    _at.write_int(addr.get_port());
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();
    _at.unlock();

    if (_at.get_last_error() == NSAPI_ERROR_OK) {
        socket->remoteAddress = addr;
        socket->connected = true;
        return NSAPI_ERROR_OK;
    }

    return NSAPI_ERROR_NO_CONNECTION;
}

nsapi_size_or_error_t UBLOX_AT_CellularStack::socket_sendto_impl(CellularSocket *socket, const SocketAddress &address,
        const void *data, nsapi_size_t size)
{
    int sent_len = 0;
    uint8_t ch;

    _at.lock();
    if (socket->proto == NSAPI_UDP) {
        _at.cmd_start("AT+USOST=");
        _at.write_int(socket->id);
        _at.write_string(address.get_ip_address(), true);
        _at.write_int(address.get_port());
        _at.write_int(size);
        _at.cmd_stop();
        wait_ms(50);
        while (ch != '@') {
          _at.read_bytes(&ch, 1);
        }
        _at.write_bytes((uint8_t *)data, size);
        
        _at.resp_start("+USOST:");
        _at.skip_param(); // skip socket id
        sent_len = _at.read_int();
        _at.resp_stop();
    } else if (socket->proto == NSAPI_TCP) {
        _at.cmd_start("AT+USOWR=");
        _at.write_int(socket->id);
        _at.write_int(size);
        _at.cmd_stop();
        wait_ms(50);
        while (ch != '@') {
          _at.read_bytes(&ch, 1);
        }
        _at.write_bytes((uint8_t *)data, size);
        
        _at.resp_start("+USOWR:");
        _at.skip_param(); // skip socket id
        sent_len = _at.read_int();
        _at.resp_stop();
    }
    _at.unlock();

    if (_at.get_last_error() == NSAPI_ERROR_OK) {
        return sent_len;
    }

    return _at.get_last_error();
}

nsapi_size_or_error_t UBLOX_AT_CellularStack::socket_recvfrom_impl(CellularSocket *socket, SocketAddress *address,
        void *buffer, nsapi_size_t size)
{
    char ipAddress[NSAPI_IP_SIZE];
    nsapi_size_or_error_t recv_len=0;
    int port;

    _at.lock();
    if (socket->proto == NSAPI_UDP) {
        _at.cmd_start("AT+USORF=");
        _at.write_int(socket->id);
        _at.write_int(size);
        _at.cmd_stop();

        _at.resp_start("+USORF:");
        _at.skip_param(); // receiving socket id
        _at.read_string(ipAddress, sizeof(ipAddress));
        port = _at.read_int();
        recv_len = _at.read_int();
        _at.read_string((char*)buffer, size);
        _at.resp_stop();
    } else if (socket->proto == NSAPI_TCP) {
        _at.cmd_start("AT+USORD=");
        _at.write_int(socket->id);
        _at.write_int(size);
        _at.cmd_stop();
        
        _at.resp_start("+USORD:");
        _at.skip_param(); // receiving socket id
        recv_len = _at.read_int();
        _at.read_bytes((uint8_t *)buffer, size);
        _at.resp_stop();
    }
    _at.unlock();


    if (!recv_len || (recv_len == -1) || (_at.get_last_error() != NSAPI_ERROR_OK)) {
        return NSAPI_ERROR_WOULD_BLOCK;
    }

    if (socket->proto == NSAPI_UDP && address) {
        address->set_ip_address(ipAddress);
        address->set_port(port);
    }

    return recv_len;
}

nsapi_error_t UBLOX_AT_CellularStack::socket_close_impl(int sock_id)
{
    _at.cmd_start("AT+USOCL=");
    _at.write_int(sock_id);
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();

    return _at.get_last_error();
}

// Find or create a socket from the list.
UBLOX_AT_CellularStack::CellularSocket * UBLOX_AT_CellularStack::find_socket(int id)
{
    CellularSocket *socket = NULL;

    for (unsigned int x = 0; (socket == NULL) && (x < sizeof(_socket) / sizeof(_socket[0])); x++) {
        if (_socket[x]->id == id) {
            socket = (_socket[x]);
        }
    }

    return socket;
}


// Clear out the storage for a socket
void UBLOX_AT_CellularStack::clear_socket(CellularSocket * socket)
{
    if (socket != NULL) {
        socket->id       = SOCKET_UNUSED;
        socket->rx_avail = 0;
        socket->_cb      = NULL;
        socket->_data    = NULL;
    }
}

const char * UBLOX_AT_CellularStack::get_ip_address()
{
    _at.lock();
    _at.cmd_start("AT+UPSND=" PROFILE ",0");
    _at.cmd_stop();

    _at.resp_start("+UPSND:");
    if (_at.info_resp()) {
        _at.skip_param();
        _at.skip_param();
        int len = _at.read_string(_ip, NSAPI_IPv4_SIZE-1);
        if (len == -1) {
            _ip[0] = '\0';
            _at.unlock();
            // no IPV4 address, return
            return NULL;
        }

        // in case stack type is not IPV4 only, try to look also for IPV6 address
        if (_stack_type != IPV4_STACK) {
            len = _at.read_string(_ip, PDP_IPV6_SIZE-1);
        }
    }
    _at.resp_stop();
    _at.unlock();
	
    // we have at least IPV4 address
    convert_ipv6(_ip);

    return _ip;
}

nsapi_error_t UBLOX_AT_CellularStack::gethostbyname(const char *host, SocketAddress *address, nsapi_version_t version)
{
    char ipAddress[NSAPI_IP_SIZE];
    nsapi_error_t err = NSAPI_ERROR_NO_CONNECTION;

    _at.lock();
    if (address->set_ip_address(host)) {
        err = NSAPI_ERROR_OK;
    } else {
        // This interrogation can sometimes take longer than the usual 8 seconds
        _at.cmd_start("AT+UDNSRN=0,");
        _at.write_string(host, true);
        _at.cmd_stop();
        
        _at.set_at_timeout(60000);
        _at.resp_start("+UDNSRN:");
        if (_at.info_resp()) {
            _at.read_string(ipAddress, sizeof(ipAddress));
            _at.resp_stop();
        
            if (address->set_ip_address(ipAddress)) {
            	err = NSAPI_ERROR_OK;
            }
        }
        _at.restore_at_timeout();
    }
    _at.unlock();

    return err;
}
