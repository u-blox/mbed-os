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
    _at->set_urc_handler("+UUSORD", mbed::Callback<void()>callback(this, &UBLOX_AT_CellularStack::UUSORD_URC));
    _at->set_urc_handler("+UUSORF", mbed::Callback<void()>callback(this, &UBLOX_AT_CellularStack::UUSORF_URC));
    _at->set_urc_handler("+UUSOCL", mbed::Callback<void()>callback(this, &UBLOX_AT_CellularStack::UUSOCL_URC));
    _at->set_urc_handler("+UUPSDD", mbed::Callback<void()>callback(this, &UBLOX_AT_CellularStack::UUPSDD_URC));
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
int UBLOX_AT_CellularStack::read_at_to_char(char * buf, int size, char end)
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
}

// Callback for Socket Read URC.
void UBLOX_AT_CellularStack::UUSORD_URC()
{
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
    }
}

// Callback for Socket Read From URC.
void UBLOX_AT_CellularStack::UUSORF_URC()
{
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
    }
}

// Callback for Socket Close URC.
void UBLOX_AT_CellularStack::UUSOCL_URC()
{
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
    }
}

// Callback for UUPSDD.
void UBLOX_AT_CellularStack::UUPSDD_URC()
{
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
    }
}

// Get the IP address of the on-board modem IP stack.
const char * UBLOX_AT_CellularStack::get_ip_address()
{
    SocketAddress address;
    LOCK();

    if (_ip == NULL) {
        // Temporary storage for an IP address string with terminator
        _ip = (char *) malloc(NSAPI_IP_SIZE);
    }

    if (_ip != NULL) {
        memset(_ip, 0, NSAPI_IP_SIZE); // Ensure a terminator
        // +UPSND=<profile_id>,<param_tag>[,<dynamic_param_val>]
        // If we get back a quoted "w.x.y.z" then we have an IP address,
        // otherwise we don't.
        if (!_at->send("AT+UPSND=" PROFILE ",0") ||
            !_at->recv("+UPSND: " PROFILE ",0,\"%" u_stringify(NSAPI_IP_SIZE) "[^\"]\"", _ip) ||
            !_at->recv("OK") ||
            !address.set_ip_address(_ip) || // Return NULL if the address is not a valid one
            !address) { // Return null if the address is zero
            free (_ip);
            _ip = NULL;
        }
    }

    UNLOCK();
    return _ip;
}



// Create a socket.
nsapi_error_t UBLOX_AT_CellularStack::socket_open(nsapi_socket_t *handle,
												 nsapi_protocol_t proto)
{
 nsapi_error_t nsapi_error = NSAPI_ERROR_DEVICE_ERROR;
 bool success = false;
 int modem_handle;
 SockCtrl *socket;
 LOCK();

 // Find a free socket
 socket = find_socket();
 tr_debug("socket_open(%d)", proto);

 if (socket != NULL) {
	 if (proto == NSAPI_UDP) {
		 success = _at->send("AT+USOCR=17");
	 } else if (proto == NSAPI_TCP) {
		 success = _at->send("AT+USOCR=6");
	 } else  {
		 nsapi_error = NSAPI_ERROR_UNSUPPORTED;
	 }

	 if (success) {
		 nsapi_error = NSAPI_ERROR_NO_SOCKET;
		 if (_at->recv("+USOCR: %d\n", &modem_handle) && (modem_handle != SOCKET_UNUSED) &&
			 _at->recv("OK")) {
			 tr_debug("Socket 0x%8x: handle %d was created", (unsigned int) socket, modem_handle);
			 clear_socket(socket);
			 socket->modem_handle         = modem_handle;
			 *handle = (nsapi_socket_t) socket;
			 nsapi_error = NSAPI_ERROR_OK;
		 }
	 }
 } else {
	 nsapi_error = NSAPI_ERROR_NO_MEMORY;
 }

 UNLOCK();
 return nsapi_error;
}

// Close a socket.
nsapi_error_t UBLOX_AT_CellularStack::socket_close(nsapi_socket_t handle)
{
    nsapi_error_t nsapi_error = NSAPI_ERROR_DEVICE_ERROR;
    SockCtrl *socket = (SockCtrl *) handle;
    LOCK();

    tr_debug("socket_close(0x%08x)", (unsigned int) handle);

    MBED_ASSERT (check_socket(socket));

    if (_at->send("AT+USOCL=%d", socket->modem_handle) &&
        _at->recv("OK")) {
        clear_socket(socket);
        nsapi_error = NSAPI_ERROR_OK;
    }

    UNLOCK();
    return nsapi_error;
}

// Bind a local port to a socket.
nsapi_error_t UBLOX_AT_CellularStack::socket_bind(nsapi_socket_t handle,
                                                    const SocketAddress &address)
{
    nsapi_error_t nsapi_error = NSAPI_ERROR_NO_SOCKET;
    int proto;
    int modem_handle;
    SockCtrl savedSocket;
    SockCtrl *socket = (SockCtrl *) handle;
    LOCK();

    tr_debug("socket_bind(0x%08x, :%d)", (unsigned int) handle, address.get_port());

    MBED_ASSERT (check_socket(socket));

    // Query the socket type
    if (_at->send("AT+USOCTL=%d,0", socket->modem_handle) &&
        _at->recv("+USOCTL: %*d,0,%d\n", &proto) &&
        _at->recv("OK")) {
        savedSocket = *socket;
        nsapi_error = NSAPI_ERROR_DEVICE_ERROR;
        // Now close the socket and re-open it with the binding given
        if (_at->send("AT+USOCL=%d", socket->modem_handle) &&
            _at->recv("OK")) {
            clear_socket(socket);
            nsapi_error = NSAPI_ERROR_CONNECTION_LOST;
            if (_at->send("AT+USOCR=%d,%d", proto, address.get_port()) &&
                _at->recv("+USOCR: %d\n", &modem_handle) && (modem_handle != SOCKET_UNUSED) &&
                _at->recv("OK")) {
                *socket = savedSocket;
                nsapi_error = NSAPI_ERROR_OK;
            }
        }
    }

    UNLOCK();
    return nsapi_error;
}

// Connect to a socket
nsapi_error_t UBLOX_AT_CellularStack::socket_connect(nsapi_socket_t handle,
                                                       const SocketAddress &address)
{
    nsapi_error_t nsapi_error = NSAPI_ERROR_DEVICE_ERROR;
    SockCtrl *socket = (SockCtrl *) handle;
    LOCK();

    tr_debug("socket_connect(0x%08x, %s(:%d))", (unsigned int) handle,
             address.get_ip_address(), address.get_port());

    MBED_ASSERT (check_socket(socket));

    if (_at->send("AT+USOCO=%d,\"%s\",%d", socket->modem_handle,
                  address.get_ip_address(), address.get_port()) &&
        _at->recv("OK")) {
        nsapi_error = NSAPI_ERROR_OK;
    }

    UNLOCK();
    return nsapi_error;
}

// Send to a socket.
nsapi_size_or_error_t UBLOX_AT_CellularStack::socket_send(nsapi_socket_t handle,
                                                            const void *data,
                                                            nsapi_size_t size)
{
    nsapi_size_or_error_t nsapi_error_size = NSAPI_ERROR_DEVICE_ERROR;
    bool success = true;
    const char *buf = (const char *) data;
    nsapi_size_t blk = MAX_WRITE_SIZE;
    nsapi_size_t count = size;
    SockCtrl *socket = (SockCtrl *) handle;

    tr_debug("socket_send(0x%08x, 0x%08x, %d)", (unsigned int) handle, (unsigned int) data, size);

    MBED_ASSERT (check_socket(socket));

    if (socket->modem_handle == SOCKET_UNUSED) {
        tr_debug("socket_send: socket closed");
        return NSAPI_ERROR_NO_SOCKET;
    }

    while ((count > 0) && success) {
        if (count < blk) {
            blk = count;
        }
        LOCK();

        if (_at->send("AT+USOWR=%d,%d", socket->modem_handle, blk) && _at->recv("@")) {
            wait_ms(50);
            if ((_at->write(buf, blk) < (int) blk) ||
                 !_at->recv("OK")) {
                success = false;
            }
        } else {
            success = false;
        }

        UNLOCK();
        buf += blk;
        count -= blk;
    }

    if (success) {
        nsapi_error_size = size - count;
        if (_debug_trace_on) {
            tr_debug("socket_send: %d \"%*.*s\"", size, size, size, (char *) data);
        }
    }

    return nsapi_error_size;
}

// Receive from a socket, TCP style.
nsapi_size_or_error_t UBLOX_AT_CellularStack::socket_recv(nsapi_socket_t handle,
                                                            void *data,
                                                            nsapi_size_t size)
{
    nsapi_size_or_error_t nsapi_error_size = NSAPI_ERROR_DEVICE_ERROR;
    bool success = true;
    char *buf = (char *) data;
    nsapi_size_t read_blk;
    nsapi_size_t count = 0;
    unsigned int usord_sz;
    int read_sz;
    Timer timer;
    SockCtrl *socket = (SockCtrl *) handle;
    int at_timeout;

    tr_debug("socket_recv(0x%08x, 0x%08x, %d)",
             (unsigned int) handle, (unsigned int) data, size);

    MBED_ASSERT (check_socket(socket));

    if (socket->modem_handle == SOCKET_UNUSED) {
        tr_debug("socket_recv: socket closed");
        return NSAPI_ERROR_NO_SOCKET;
    }

    timer.start();

    while (success && (size > 0)) {
        LOCK();
        at_timeout = _at_timeout;
        at_set_timeout(1000);

        read_blk = MAX_READ_SIZE;
        if (read_blk > size) {
            read_blk = size;
        }
        if (socket->pending > 0) {
            tr_debug("Socket 0x%08x: modem handle %d has %d byte(s) pending",
                     (unsigned int) socket, socket->modem_handle, socket->pending);
            _at->debug_on(false); // ABSOLUTELY no time for debug here if you want to
                                  // be able to read packets of any size without
                                  // losing characters in UARTSerial
            if (_at->send("AT+USORD=%d,%d", socket->modem_handle, read_blk) &&
                _at->recv("+USORD: %*d,%d,\"", &usord_sz)) {
                // Must use what +USORD returns here as it may be less or more than we asked for
                if (usord_sz > socket->pending) {
                    socket->pending = 0;
                } else {
                    socket->pending -= usord_sz;
                }
                // Note: insert no debug between _at->recv() and _at->read(), no time...
                if (usord_sz > size) {
                    usord_sz = size;
                }
                read_sz = _at->read(buf, usord_sz);
                if (read_sz > 0) {
                    tr_debug("...read %d byte(s) from modem handle %d...", read_sz,
                             socket->modem_handle);
                    if (_debug_trace_on) {
                        tr_debug("Read returned %d,  |%*.*s|", read_sz, read_sz, read_sz, buf);
                    }
                    count += read_sz;
                    buf += read_sz;
                    size -= read_sz;
                } else {
                    // read() should not fail
                    success = false;
                }
                tr_debug("Socket 0x%08x: modem handle %d now has only %d byte(s) pending",
                         (unsigned int) socket, socket->modem_handle, socket->pending);
                // Wait for the "OK" before continuing
                _at->recv("OK");
            } else {
                // Should never fail to do _at->send()/_at->recv()
                success = false;
            }
            _at->debug_on(_debug_trace_on);
        } else if (timer.read_ms() < SOCKET_TIMEOUT) {
            // Wait for URCs
            _at->recv(UNNATURAL_STRING);
        } else {
            if (count == 0) {
                // Timeout with nothing received
                nsapi_error_size = NSAPI_ERROR_WOULD_BLOCK;
                success = false;
            }
            size = 0; // This simply to cause an exit
        }

        at_set_timeout(at_timeout);
        UNLOCK();
    }
    timer.stop();

    if (success) {
        nsapi_error_size = count;
    }

    if (_debug_trace_on) {
        tr_debug("socket_recv: %d \"%*.*s\"", count, count, count, buf - count);
    } else {
        tr_debug("socket_recv: received %d byte(s)", count);
    }

    return nsapi_error_size;
}

// Send to an IP address.
nsapi_size_or_error_t UBLOX_AT_CellularStack::socket_sendto(nsapi_socket_t handle,
                                                              const SocketAddress &address,
                                                              const void *data,
                                                              nsapi_size_t size)
{
    nsapi_size_or_error_t nsapi_error_size = NSAPI_ERROR_DEVICE_ERROR;
    bool success = true;
    const char *buf = (const char *) data;
    nsapi_size_t blk = MAX_WRITE_SIZE;
    nsapi_size_t count = size;
    SockCtrl *socket = (SockCtrl *) handle;

    tr_debug("socket_sendto(0x%8x, %s(:%d), 0x%08x, %d)", (unsigned int) handle,
             address.get_ip_address(), address.get_port(), (unsigned int) data, size);

    MBED_ASSERT (check_socket(socket));

    if (size > MAX_WRITE_SIZE) {
        tr_warn("WARNING: packet length %d is too big for one UDP packet (max %d), will be fragmented.", size, MAX_WRITE_SIZE);
    }

    while ((count > 0) && success) {
        if (count < blk) {
            blk = count;
        }
        LOCK();

        if (_at->send("AT+USOST=%d,\"%s\",%d,%d", socket->modem_handle,
                      address.get_ip_address(), address.get_port(), blk) &&
            _at->recv("@")) {
            wait_ms(50);
            if ((_at->write(buf, blk) >= (int) blk) &&
                 _at->recv("OK")) {
            } else {
                success = false;
            }
        } else {
            success = false;
        }

        UNLOCK();
        buf += blk;
        count -= blk;
    }

    if (success) {
        nsapi_error_size = size - count;
        if (_debug_trace_on) {
            tr_debug("socket_sendto: %d \"%*.*s\"", size, size, size, (char *) data);
        }
    }

    return nsapi_error_size;
}

// Receive a packet over a UDP socket.
nsapi_size_or_error_t UBLOX_AT_CellularStack::socket_recvfrom(nsapi_socket_t handle,
                                                                SocketAddress *address,
                                                                void *data,
                                                                nsapi_size_t size)
{
    nsapi_size_or_error_t nsapi_error_size = NSAPI_ERROR_DEVICE_ERROR;
    bool success = true;
    char *buf = (char *) data;
    nsapi_size_t read_blk;
    nsapi_size_t count = 0;
    char ipAddress[NSAPI_IP_SIZE];
    int port;
    unsigned int usorf_sz;
    int read_sz;
    Timer timer;
    SockCtrl *socket = (SockCtrl *) handle;
    int at_timeout;

    tr_debug("socket_recvfrom(0x%08x, 0x%08x, %d)",
             (unsigned int) handle, (unsigned int) data, size);

    MBED_ASSERT (check_socket(socket));

    timer.start();

    while (success && (size > 0)) {
        LOCK();
        at_timeout = _at_timeout;
        at_set_timeout(1000);

        read_blk = MAX_READ_SIZE;
        if (read_blk > size) {
            read_blk = size;
        }
        if (socket->pending > 0) {
            tr_debug("Socket 0x%08x: modem handle %d has %d byte(s) pending",
                     (unsigned int) socket, socket->modem_handle, socket->pending);
            memset (ipAddress, 0, sizeof (ipAddress)); // Ensure terminator

            // Note: the maximum length of UDP packet we can receive comes from
            // fitting all of the following into one buffer:
            //
            // +USORF: xx,"max.len.ip.address.ipv4.or.ipv6",yyyyy,wwww,"the_data"\r\n
            //
            // where xx is the handle, max.len.ip.address.ipv4.or.ipv6 is NSAPI_IP_SIZE,
            // yyyyy is the port number (max 65536), wwww is the length of the data and
            // the_data is binary data. I make that 29 + 48 + len(the_data),
            // so the overhead is 77 bytes.

            _at->debug_on(false); // ABSOLUTELY no time for debug here if you want to
                                  // be able to read packets of any size without
                                  // losing characters in UARTSerial
            if (_at->send("AT+USORF=%d,%d", socket->modem_handle, read_blk) &&
                _at->recv("+USORF: %*d,\"%" u_stringify(NSAPI_IP_SIZE) "[^\"]\",%d,%d,\"",
                          ipAddress, &port, &usorf_sz)) {
                // Must use what +USORF returns here as it may be less or more than we asked for
                if (usorf_sz > socket->pending) {
                    socket->pending = 0;
                } else {
                    socket->pending -= usorf_sz;
                }
                // Note: insert no debug between _at->recv() and _at->read(), no time...
                if (usorf_sz > size) {
                    usorf_sz = size;
                }
                read_sz = _at->read(buf, usorf_sz);
                if (read_sz > 0) {
                    address->set_ip_address(ipAddress);
                    address->set_port(port);
                    tr_debug("...read %d byte(s) from modem handle %d...", read_sz,
                             socket->modem_handle);
                    if (_debug_trace_on) {
                        tr_debug("Read returned %d,  |%*.*s|", read_sz, read_sz, read_sz, buf);
                    }
                    count += read_sz;
                    buf += read_sz;
                    size -= read_sz;
                    if ((usorf_sz < read_blk) || (usorf_sz == MAX_READ_SIZE)) {
                        size = 0; // If we've received less than we asked for, or
                                  // the max size, then a whole UDP packet has arrived and
                                  // this means DONE.
                    }
                } else {
                    // read() should not fail
                    success = false;
                }
                tr_debug("Socket 0x%08x: modem handle %d now has only %d byte(s) pending",
                         (unsigned int) socket, socket->modem_handle, socket->pending);
                // Wait for the "OK" before continuing
                _at->recv("OK");
            } else {
                // Should never fail to do _at->send()/_at->recv()
                success = false;
            }
            _at->debug_on(_debug_trace_on);
        } else if (timer.read_ms() < SOCKET_TIMEOUT) {
            // Wait for URCs
            _at->recv(UNNATURAL_STRING);
        } else {
            if (count == 0) {
                // Timeout with nothing received
                nsapi_error_size = NSAPI_ERROR_WOULD_BLOCK;
                success = false;
            }
            size = 0; // This simply to cause an exit
        }

        at_set_timeout(at_timeout);
        UNLOCK();
    }
    timer.stop();

    if (success) {
        nsapi_error_size = count;
    }

    if (_debug_trace_on) {
        tr_debug("socket_recvfrom: %d \"%*.*s\"", count, count, count, buf - count);
    } else {
        tr_debug("socket_recvfrom: received %d byte(s)", count);
    }

    return nsapi_error_size;
}

// Attach an event callback to a socket, required for asynchronous
// data reception
void UBLOX_AT_CellularStack::socket_attach(nsapi_socket_t handle,
                                             void (*callback)(void *),
                                             void *data)
{
    SockCtrl *socket = (SockCtrl *) handle;

    MBED_ASSERT (check_socket(socket));

    socket->callback = callback;
    socket->data = data;
}



