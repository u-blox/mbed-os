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
#ifndef UBLOX_ATHANDLER_H_
#define UBLOX_ATHANDLER_H_

#include "ATHandler.h"

namespace mbed {

/**
 *  Class UBLOX_ATHandler 
 *
 */
class UBLOX_ATHandler : public ATHandler
{
public:
    UBLOX_ATHandler(FileHandle *fh, events::EventQueue &queue, int timeout, const char *output_delimiter, uint16_t send_delay = 0);

    /** Starts the command writing by clearing the last error and writing the given command.
     *  In case of failure when writing, the last error is set to NSAPI_ERROR_DEVICE_ERROR.
     *
     *  @param cmd  AT command to be written to modem
     */
    virtual void cmd_start(const char *cmd);

    bool _idle_mode_status = false;
};


} // namespace mbed

#endif /* AT_CELLULAR_BASE_H_ */
