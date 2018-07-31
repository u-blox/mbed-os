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

#ifndef UBLOX_AT_CELLULAR_DEVICE_H_
#define UBLOX_AT_CELLULAR_DEVICE_H_

#include "AT_CellularDevice.h"

#include "AT_CellularNetwork.h"
#include "AT_CellularSIM.h"
#include "AT_CellularSMS.h"
#include "AT_CellularPower.h"
#include "AT_CellularInformation.h"

#include "UBLOX_ATHandler.h"

namespace mbed
{

/**
 *  Class AT_CellularDevice
 *
 *  A class defines opening and closing of cellular interfaces.
 *  Deleting/Closing of opened interfaces can be done only through this class.
 */
class UBLOX_AT_CellularDevice : public AT_CellularDevice
{
public:
    UBLOX_AT_CellularDevice(events::EventQueue &queue);

protected:
    //UBLOX_ATHandler *_atHandlers;

    ATHandler *get_at_handler(FileHandle *fh);

    /** Releases the given at_handler. If last reference to at_hander then it's deleted.
     *
     *  @param at_handler
     */
    void release_at_handler(ATHandler* at_handler);
};

} // namespace mbed
#endif // AT_CELLULAR_DEVICE_H_
