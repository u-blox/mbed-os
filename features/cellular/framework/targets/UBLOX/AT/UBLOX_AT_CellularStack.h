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
#ifndef UBLOX_AT_CELLULARSTACK_H_
#define UBLOX_AT_CELLULARSTACK_H_

#include "AT_CellularStack.h"
#include "CellularUtil.h"

//#define BC95_SOCKET_MAX 7
//#define BC95_MAX_PACKET_SIZE 512

namespace mbed {

class UBLOX_AT_CellularStack : public AT_CellularStack
{
public:
    UBLOX_AT_CellularStack(ATHandler &atHandler, int cid, nsapi_ip_stack_t stack_type);
    virtual ~UBLOX_AT_CellularStack();


protected: // AT_CellularStack

    /** Lock a mutex when accessing the modem.
     */
    void lock(void)     { _mtx.lock(); }

    /** Helper to make sure that lock unlock pair is always balanced
     */
    #define LOCK()         { lock()

    /** Unlock the modem when done accessing it.
     */
    void unlock(void)   { _mtx.unlock(); }

    /** Helper to make sure that lock unlock pair is always balanced
     */
    #define UNLOCK()       } unlock()

    virtual const char *get_ip_address();

    virtual nsapi_error_t socket_open(nsapi_socket_t *handle, nsapi_protocol_t proto);

    virtual nsapi_error_t socket_close(nsapi_socket_t handle);

    virtual nsapi_error_t socket_bind(nsapi_socket_t handle, const SocketAddress &address);

    // Unsupported TCP server function.
    virtual nsapi_error_t socket_listen(nsapi_socket_t handle, int backlog);

    virtual nsapi_error_t socket_connect(nsapi_socket_t handle, const SocketAddress &address);

    // Unsupported TCP server function.
    virtual nsapi_error_t socket_accept(nsapi_socket_t server, nsapi_socket_t *handle, SocketAddress *address=0);

    virtual nsapi_size_or_error_t socket_send(nsapi_socket_t handle, const void *data, nsapi_size_t size);

    virtual nsapi_size_or_error_t socket_recv(nsapi_socket_t handle, void *data, nsapi_size_t size);

    virtual nsapi_size_or_error_t socket_sendto(nsapi_socket_t handle, const SocketAddress &address, const void *data, nsapi_size_t size);

    virtual nsapi_size_or_error_t socket_recvfrom(nsapi_socket_t handle, SocketAddress *address, void *buffer, nsapi_size_t size);

    virtual void socket_attach(nsapi_socket_t handle, void (*callback)(void *), void *data);

    int read_at_to_char(char * buf, int size, char end);

private:
    // URC handlers
    void UUSORD_URC();
    void UUSORF_URC();
    void UUSOCL_URC();
    void UUPSDD_URC();
};
} // namespace mbed
#endif /* UBLOX_AT_CELLULARSTACK_H_ */
