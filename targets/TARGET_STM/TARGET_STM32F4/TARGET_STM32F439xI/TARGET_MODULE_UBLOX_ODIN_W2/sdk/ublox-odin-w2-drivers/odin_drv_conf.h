/* ODIN-W2 user Config Interface
 * Copyright (c) 2016 u-blox Malmö AB
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

#ifndef ODIN_DRV_CONFIG_H
#define ODIN_DRV_CONFIG_H

/** Enum of WiFi Configuration types
 *
 *  The configuration type specifies a particular parameter of
 *  a WiFi interface. And will be used particular configuration to 
 *  use when initializing a WiFi network e.g. 802.11r, 802.11w
 *
 */

typedef enum target_config_params_e {
    ODIN_CFG_FIRST,
    ODIN_CFG_SET_POWER_SAVE_MODE = ODIN_CFG_FIRST,       //!< Set power mode  @ref cbWLAN_IoctlPowerSaveMode
    ODIN_CFG_GET_POWER_SAVE_MODE,                            //!< Get power mode  @ref cbWLAN_IoctlPowerSaveMode
    ODIN_CFG_SET_LISTEN_INTERVAL,                            //!< Set listen interval, integer value 0 - 16 
    ODIN_CFG_GET_LISTEN_INTERVAL,                            //!< Get listen interval, integer value 0 - 16 
    ODIN_CFG_SET_DTIM_ENABLE,                                //!< Set DTIM enable 0, disable 1 enable
    ODIN_CFG_GET_DTIM_ENABLE,                                //!< Get DTIM enable 0, disable 1 enable
    ODIN_CFG_SET_SLEEP_TIMEOUT,                              //!< Set enter power save entry delay (in ms). Power save mode will be entered only if there no activity during this delay
    ODIN_CFG_GET_SLEEP_TIMEOUT,                              //!< Get enter power save entry delay (in ms). Power save mode will be entered only if there no activity during this delay
    ODIN_CFG_SET_GOOD_RSSI_YIELD_TMO,
    ODIN_CFG_GET_GOOD_RSSI_YIELD_TMO,
    ODIN_CFG_SET_BAD_RSSI_YIELD_TMO,
    ODIN_CFG_GET_BAD_RSSI_YIELD_TMO,
	ODIN_CFG_SET_PMF_STA,
	ODIN_CFG_GET_PMF_STA,
	ODIN_CFG_SET_FT_MODE,
	ODIN_CFG_GET_FT_MODE,
    ODIN_CFG_LAST,
} target_config_params_e;

/**
* Power save modes set using  @ref cbWLAN_ioctl
*
* @ingroup wlan
*/
typedef enum {
    ODIN_POWER_SAVE_MODE_OFF,
    ODIN_POWER_SAVE_MODE_SLEEP,
    ODIN_POWER_SAVE_MODE_DEEP_SLEEP
} target_power_save_mode_e;

#endif

