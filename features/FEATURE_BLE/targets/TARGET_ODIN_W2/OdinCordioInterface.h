/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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

#ifndef ODIN_CORDIO_INTF_H
#define ODIN_CORDIO_INTF_H

#include <stdio.h>
#include "mbed.h"

/*------------------------------------------------------------------------------
*  Vendor specific commands opcode 
* ------------------------------------------------------------------------------
*/

/* Command to write hardware address to BT device */
#define HCID_CC_TI_WRITE_BD_ADDR            0xFC06

/* It configures clk parameters for fast and slow clock */
#define HCID_CC_TI_FAST_CLOCK_CONFIG_BTIP   0xFD1C

/* Command to control behavior of HCILL deep-sleep protocol */
#define HCID_CC_TI_HCILL_PARS_CFG           0xFD2B

/* Command to configures the sleep mode */
#define HCID_CC_TI_SLEEP_PROTOCOLS_CFG      0xFD0C

#define HCI_RESET_RAND_CNT                  4


/*------------------------------------------------------------------------------
* External variables and functions 
* ------------------------------------------------------------------------------
*/
extern const unsigned char  g_ServicePack[];
extern "C" unsigned int getSizeOfServicePack();

extern void cbCordio_Btinit(void);
extern void send_hci_vs_cmd(uint16_t opcode);

#endif /* ODIN_CORDIO_INTF_H */
