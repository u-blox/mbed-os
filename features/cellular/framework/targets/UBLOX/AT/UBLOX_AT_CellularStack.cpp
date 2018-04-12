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

using namespace mbed;
using namespace mbed_cellular_util;

UBLOX_AT_CellularStack::UBLOX_AT_CellularStack(ATHandler &atHandler, int cid, nsapi_ip_stack_t stack_type) : AT_CellularStack(atHandler, cid, stack_type)
{
    // URC handlers for sockets
    //_at->set_urc_handler("+UUSORD", mbed::Callback<void()>callback(this, &UBLOX_AT_CellularStack::UUSORD_URC));
    //_at->set_urc_handler("+UUSORF", mbed::Callback<void()>callback(this, &UBLOX_AT_CellularStack::UUSORF_URC));
    //_at->set_urc_handler("+UUSOCL", mbed::Callback<void()>callback(this, &UBLOX_AT_CellularStack::UUSOCL_URC));
    //_at->set_urc_handler("+UUPSDD", mbed::Callback<void()>callback(this, &UBLOX_AT_CellularStack::UUPSDD_URC));
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

// Read up to size bytes from the AT interface up to a "end".
// Note: the AT interface should be locked before this is called.
/*int UBLOX_AT_CellularStack::read_at_to_char(char * buf, int size, char end)
{
    int count = 0;
   int x = 0;

    if (size > 0) {
        for (count = 0; (count < size) && (x >= 0) && (x != end); count++) {
            x = _at->getc();
            *(buf + count) = (char) x;
        }

        count--;
        *(buf + count) = 0;

        // Convert line endings:
        // If end was '\n' (0x0a) and the preceding character was 0x0d, then
        // overwrite that with null as well.
        if ((count > 0) && (end == '\n') && (*(buf + count - 1) == '\x0d')) {
            count--;
            *(buf + count) = 0;
        }
    }

    return count;
}*/

// Callback for Socket Read URC.
void UBLOX_AT_CellularStack::UUSORD_URC()
{/*
    int a;
    int b;
    char buf[32];
    SockCtrl *socket;

    // Note: not calling _at->recv() from here as we're
    // already in an _at->recv()
    // +UUSORD: <socket>,<length>
    if (read_at_to_char(buf, sizeof (buf), '\n') > 0) {
        if (sscanf(buf, ": %d,%d", &a, &b) == 2) {
            socket = find_socket(a);
            if (socket != NULL) {
                socket->pending = b;
                // No debug prints here as they can affect timing
                // and cause data loss in UARTSerial
                if (socket->callback != NULL) {
                    socket->callback(socket->data);
                }
            }
        }
    }*/
}

// Callback for Socket Read From URC.
void UBLOX_AT_CellularStack::UUSORF_URC()
{/*
    int a;
    int b;
    char buf[32];
    SockCtrl *socket;

    // Note: not calling _at->recv() from here as we're
    // already in an _at->recv()
    // +UUSORF: <socket>,<length>
    if (read_at_to_char(buf, sizeof (buf), '\n') > 0) {
        if (sscanf(buf, ": %d,%d", &a, &b) == 2) {
            socket = find_socket(a);
            if (socket != NULL) {
                socket->pending = b;
                // No debug prints here as they can affect timing
                // and cause data loss in UARTSerial
                if (socket->callback != NULL) {
                    socket->callback(socket->data);
                }
            }
        }
    }*/
}

// Callback for Socket Close URC.
void UBLOX_AT_CellularStack::UUSOCL_URC()
{/*
    int a;
    char buf[32];
    SockCtrl *socket;

    // Note: not calling _at->recv() from here as we're
    // already in an _at->recv()
    // +UUSOCL: <socket>
    if (read_at_to_char(buf, sizeof (buf), '\n') > 0) {
        if (sscanf(buf, ": %d", &a) == 1) {
            socket = find_socket(a);
            tr_debug("Socket 0x%08x: handle %d closed by remote host",
                     (unsigned int) socket, a);
            clear_socket(socket);
        }
    }*/
}

// Callback for UUPSDD.
void UBLOX_AT_CellularStack::UUPSDD_URC()
{/*
    int a;
    char buf[32];
    SockCtrl *socket;

    // Note: not calling _at->recv() from here as we're
    // already in an _at->recv()
    // +UUPSDD: <socket>
    if (read_at_to_char(buf, sizeof (buf), '\n') > 0) {
        if (sscanf(buf, ": %d", &a) == 1) {
            socket = find_socket(a);
            tr_debug("Socket 0x%08x: handle %d connection lost",
                     (unsigned int) socket, a);
            clear_socket(socket);
            if (_connection_status_cb) {
                _connection_status_cb(NSAPI_ERROR_CONNECTION_LOST);
            }
        }
    }*/
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
	//TODO: ADD TCP here
    return (protocol == NSAPI_UDP);
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

nsapi_error_t UBLOX_AT_CellularStack::create_socket_impl(CellularSocket *socket)
{
    int sock_id, response_length = -1;
    bool socketOpenWorking = false;

    if (socket->proto == NSAPI_UDP) {

    	char response_buffer[UBLOX_U201_AT_COMMAND_BUFFER_SIZE];

        _at.cmd_start("AT+USOCR=17,");
        _at.write_int(socket->localAddress.get_port());
        _at.cmd_stop();
        _at.resp_start();
        response_length = _at.read_string(response_buffer, UBLOX_U201_AT_COMMAND_BUFFER_SIZE);
        // TODO parse string
        sock_id = sock_id;
        _at.resp_stop();

        socketOpenWorking = (_at.get_last_error() == NSAPI_ERROR_OK);

        if (!socketOpenWorking) {
        	/* No retries will return NSAPI_ERROR_NO_SOCKET
            _at.cmd_start("AT+NSOCL=0");
            _at.cmd_stop();
            _at.resp_start();
            _at.resp_stop();

            _at.cmd_start("AT+NSOCR=DGRAM,17,");
            _at.write_int(socket->localAddress.get_port());
            _at.write_int(1);
            _at.cmd_stop();
            _at.resp_start();
            sock_id = _at.read_int();
            _at.resp_stop();

            socketOpenWorking = (_at.get_last_error() == NSAPI_ERROR_OK);*/
        }
    }
    else if(socket->proto == NSAPI_TCP)
    {
    	// TODO
    }

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

nsapi_size_or_error_t UBLOX_AT_CellularStack::socket_sendto_impl(CellularSocket *socket, const SocketAddress &address,
        const void *data, nsapi_size_t size)
{
    int sent_len = 0;

    char hexstr[UBLOX_U201_MAX_PACKET_SIZE*2 + 1] = {0};
    char_str_to_hex_str((const char*)data, size, hexstr);

    _at.cmd_start("AT+USOST=");
    _at.write_int(socket->id);
    _at.write_string(address.get_ip_address(), false);
    _at.write_int(address.get_port());
    _at.write_int(size);
    _at.write_string(hexstr, false);
    _at.cmd_stop();
    _at.resp_start();
    // skip socket id
    _at.skip_param();
    sent_len = _at.read_int();
    _at.resp_stop();

    if (_at.get_last_error() == NSAPI_ERROR_OK) {
        return sent_len;
    }

    return _at.get_last_error();
}

nsapi_size_or_error_t UBLOX_AT_CellularStack::socket_recvfrom_impl(CellularSocket *socket, SocketAddress *address,
        void *buffer, nsapi_size_t size)
{
    nsapi_size_or_error_t recv_len=0;
    int port;
    char ip_address[NSAPI_IP_SIZE];
    char hexstr[UBLOX_U201_MAX_PACKET_SIZE*2 + 1];

    _at.cmd_start("AT+USORF=");
    _at.write_int(socket->id);
    _at.write_int(size);
    _at.cmd_stop();
    _at.resp_start();
    // receiving socket id
    _at.skip_param();
    _at.read_string(ip_address, sizeof(ip_address));
    port = _at.read_int();
    recv_len = _at.read_int();
    _at.read_string(hexstr, sizeof(hexstr));
    // remaining length
    _at.skip_param();
    _at.resp_stop();

    if (!recv_len || (recv_len == -1) || (_at.get_last_error() != NSAPI_ERROR_OK)) {
        return NSAPI_ERROR_WOULD_BLOCK;
    }

    if (address) {
        address->set_ip_address(ip_address);
        address->set_port(port);
    }

    if (recv_len > 0) {
        hex_str_to_char_str((const char*) hexstr, recv_len*2, (char*)buffer);
    }

    return recv_len;
}
