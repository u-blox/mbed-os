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

#include "UBLOX_ATHandler.h"
#include "rtos/Thread.h"

using namespace mbed;

UBLOX_ATHandler::UBLOX_ATHandler(FileHandle *fh, events::EventQueue &queue, int timeout, const char *output_delimiter, uint16_t send_delay) : ATHandler(fh, queue, timeout, output_delimiter, send_delay)
{

}

void UBLOX_ATHandler::cmd_start(const char *cmd)
{
    uint8_t time_out=20; //wait a maximum of 1 second? //do we need a lock?

    //if (_idle_mode_status == false) { //idle mode is active: IMPORTANT: this method cannot work as UPSV settings are retained after power reset.
        while(true && time_out--)
        {
            ATHandler::cmd_start("AT");
            ATHandler::cmd_stop();
            ATHandler::resp_start();
            ATHandler::resp_stop();
            if (ATHandler::get_last_error() == NSAPI_ERROR_OK)
            {
                break;              
            }
            else
            {
                rtos::Thread::wait_until(50);  
            }
        }
    //}
    ATHandler::cmd_start(cmd);
}

