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

#include "UBLOX_AT_CellularDevice.h"

using namespace events;
using namespace mbed;

UBLOX_AT_CellularDevice::UBLOX_AT_CellularDevice(EventQueue &queue) : AT_CellularDevice(queue)
{
}

// each parser is asslociated with one filehandle (that is UART)
ATHandler *UBLOX_AT_CellularDevice::get_at_handler(FileHandle *fileHandle)
{
    if (!fileHandle) {
        return NULL;
    }
    ATHandler *atHandler = _atHandlers;
    while (atHandler) {
        if (atHandler->get_file_handle() == fileHandle) {
            atHandler->inc_ref_count();
            return atHandler;
        }
        atHandler = (atHandler->_nextATHandler);
    }

    atHandler = new UBLOX_ATHandler(fileHandle, _queue, _default_timeout, "\r", get_send_delay());
    if (atHandler) {
        if (_modem_debug_on) {
            atHandler->set_debug(_modem_debug_on);
        }
        atHandler->_nextATHandler = _atHandlers;
        _atHandlers = atHandler;
    }

    return atHandler;
}

void UBLOX_AT_CellularDevice::release_at_handler(ATHandler *at_handler)
{
    if (!at_handler) {
        return;
    }
    at_handler->dec_ref_count();
    if (at_handler->get_ref_count() == 0) {
        // we can delete this at_handler
        ATHandler *atHandler = _atHandlers;
        ATHandler *prev = NULL;
        while (atHandler) {
            if (atHandler == at_handler) {
                if (prev == NULL) {
                    _atHandlers = (_atHandlers->_nextATHandler);
                } else {
                    prev->_nextATHandler = atHandler->_nextATHandler;
                }
                delete atHandler;
                break;
            } else {
                prev = atHandler;
                atHandler = (atHandler->_nextATHandler);
            }
        }
    }
}
