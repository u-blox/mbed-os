#ifndef HCI_DRIVER_ODIN_H
#define HCI_DRIVER_ODIN_H
 /*------------------------------------------------------------------------------
 * Copyright (c) 2007 connectBlue AB, Sweden.
 * Any reproduction without written permission is prohibited by law.
 *------------------------------------------------------------------------------
 * Produced by connectBlue AB
 *------------------------------------------------------------------------------
 * Component: Microstack 
 * File     : 
 * Revision :
 *
 * Date     : 
 * Author   :
 *------------------------------------------------------------------------------
 * Description: 
 *------------------------------------------------------------------------------
 */

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

#endif /* HCI_DRIVER_ODIN_H */