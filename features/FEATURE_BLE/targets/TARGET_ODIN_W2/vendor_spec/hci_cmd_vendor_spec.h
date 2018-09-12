#ifndef HCI_COMMAND_VS_H
#define HCI_COMMAND_VS_H
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
#include "hci_defines_vendor_specific.h"
#include "hci_defs.h"
#include "hci_api.h"
#include "hci_cmd.h"
#include "hci_core.h"

/*==============================================================================
 * CONSTANTS
 *==============================================================================
 */

/*==============================================================================
 * TYPES
 *==============================================================================
 */

/*=============================================================================
 * EXPORTED FUNCTIONS
 *=============================================================================
 */

#if (BLE_STACK_CORDIO)

/* Vendor Specific Commands need to be implemented in separate file */
void vs_cmd_writeBdAddress(void);

void vs_cmd_fast_clk_config(
  uint8_t xtalEnableStatus,
  uint32_t normalWakeupSettlingTime,
  uint32_t fastWakeupSettlingTime,
  uint8_t fastWakeupEnable,
  uint8_t xtalBoostGain,
  uint8_t xtalNormalGain,
  uint8_t bluetoothTxSlicerTrim,
  uint8_t bluetoothIdleSlicerTrim,
  uint8_t fastClockInputAcDc,
  uint8_t slowClockAccuracy,
  uint8_t clockSource,
  uint8_t gcmExtraSettlingTime,
  uint8_t reserved);

void vs_cmd_ti_ll_pars(
    uint16_t   inactivityTimeout,
    uint16_t   retransmitTimeout,
    uint8_t    rtsPulseWidth);

void vs_cmd_sleep_prot_config(
    uint8_t    bigSleepEnable,
    uint8_t    deepSleepEnable,
    uint8_t    deepSleepProtMode,
    uint8_t    outputIoSelect,
    uint8_t    outputPullEnable,
    uint8_t    inputPullEnable,
    uint8_t    inputIoSelect,
    uint16_t   hostWakeDeassertionTimer);

#endif
#endif

