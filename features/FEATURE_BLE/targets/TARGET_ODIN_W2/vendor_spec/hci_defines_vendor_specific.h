#ifndef _HCI_HC_VS_DEFINES_H
#define _HCI_HC_VS_DEFINES_H
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
#include "hci_api.h"
#include "hci_cmd.h"
#include "hci_core.h"
#include "bstream.h"
#include "wsf_buf.h"
#include <stdbool.h>
/*==============================================================================
 * CONSTANTS
 *==============================================================================
 */

/* OP CODES */
#if (HCI_VENDOR_SPECIFIC_SUPPORT_TI == 1)
#define HCID_CC_TI_BLE_OUTPUT_POWER                               0xFDDD
#define HCID_CC_TI_UPDATE_UART_BAUD_RATE                          0xFF36
#define HCID_CC_TI_WRITE_BD_ADDR                                  0xFC06
#define HCID_CC_TI_CONTINUOUS_RX                                  0xFD17
#define HCID_CC_TI_SPI_CONFIGURATION                              0xFD41
#define HCID_CC_TI_CONTINUOUS_TX                                  0xFD84
#define HCID_CC_TI_PACKET_TX_RX                                   0xFD85
#define HCID_CC_TI_SLEEP_PROTOCOLS_CFG                            0xFD0C
#define HCID_CC_TI_FAST_CLOCK_CONFIG_BTIP                         0xFD1C
#define HCID_CC_TI_HCILL_PARS_CFG                                 0xFD2B
#define HCID_CC_TI_SET_QOS_INTERVAL                               0xFF33
#define HCID_CC_TI_WRITE_CODEC_CONFIG_BTI                         0xFD06
#define HCID_CC_TI_WRITE_CODEC_CONFIG_ENHANCED                    0xFD07
#define HCID_CC_TI_SET_PCM_LOOPBACK_ENABLE                        0xFE28
#define HCID_CC_TI_SET_POWER_VECTOR                               0xFD82
#define HCID_CC_TI_SET_CALIBRATION_INFO                           0xFD76
#define HCID_CC_TI_SET_LE_TEST_MODE_PARAMS                        0xFD77
#define HCID_CC_TI_ENABLE_RF_CALIBRATION                          0xFD80
#define HCID_CC_TI_CONFIG_DBG_MSG_OVER_HCI                        0xFD7E
#define HCID_CC_TI_ENABLE_PROTOCOL_VIEWER                         0xFF68
#define HCID_CC_TI_WRITE_HARDWARE_REGISTER                        0xFF01
#define HCID_CC_TI_DRPB_RESET                                     0xFD88
#define HCID_CC_TI_SYSTEM_STATUS                                  0xFE1F

#define HCID_CC_TI_DRPb_Tester_Con_TX                             0xFDCA
#define HCID_CC_TI_DRPb_Tester_Con_RX                             0xFDCB
#define HCID_CC_TI_DRPb_Tester_Packet_TX_RX                       0xFDCC
#endif

#endif
