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

#include "UBLOX_AT_CellularPower.h"

#include "onboard_modem_api.h"

using namespace mbed;

UBLOX_AT_CellularPower::UBLOX_AT_CellularPower(ATHandler &atHandler) : AT_CellularPower(atHandler)
{

}

UBLOX_AT_CellularPower::~UBLOX_AT_CellularPower()
{
   
}

nsapi_error_t UBLOX_AT_CellularPower::on()
{
#if MODEM_ON_BOARD
    ::onboard_modem_init();
    ::onboard_modem_power_up();
#endif
    return NSAPI_ERROR_OK;
}

nsapi_error_t UBLOX_AT_CellularPower::off()
{
#if MODEM_ON_BOARD
    ::onboard_modem_power_down();
#endif
    return NSAPI_ERROR_OK;
}

nsapi_error_t UBLOX_AT_CellularPower::set_idle_mode(int idle_mode_value)
{
    _at.lock();
    _at.cmd_start("AT+UPSV=");
    _at.write_int(idle_mode_value); //0,4 for R410.
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();

    if (_at.get_last_error() == NSAPI_ERROR_OK) {
        if (idle_mode_value != 0) {
        	_at._idle_mode_status = true;
        }
        else {
        	_at._idle_mode_status = false;
        }

        _at.unlock();
        return NSAPI_ERROR_OK;
    }
    return _at.unlock_return_error(); //what if last error changes as mutex is released
}
