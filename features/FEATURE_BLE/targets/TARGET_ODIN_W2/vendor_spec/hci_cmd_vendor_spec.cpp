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

#include "stdint.h"
#include "hci_cmd_vendor_spec.h"

/*=============================================================================
 * INTERFACE ROUTINES
 *=============================================================================
 */

#if (BLE_STACK_CORDIO)
    
/* Vendor Specific Commands need to be implemented in separate file */
void vs_cmd_writeBdAddress(void)
{
	uint8_t		Bt_Address[] = {0xd4, 0xca, 0x6e, 0x70, 0x4e, 0x3c};	// (TODO) This needs to come from app
	uint8_t 	*packet = hciCmdAlloc(HCID_CC_TI_WRITE_BD_ADDR, 6);
	if (packet) {
		memcpy(packet +  HCI_CMD_HDR_LEN, Bt_Address, 6);
		hciCmdSend(packet);
	} else {
		printf("Error cannot allocate memory for the buffer");
	}
}

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
    uint8_t reserved)
{
    uint8_t*   packet;

    packet = hciCmdAlloc(HCID_CC_TI_FAST_CLOCK_CONFIG_BTIP, 19);  
    if(packet){
        packet += HCI_CMD_HDR_LEN;
        packet[0] = xtalEnableStatus;

        packet[1] = (uint8_t)(normalWakeupSettlingTime & 0xFF);
        packet[2] = (uint8_t)(normalWakeupSettlingTime >> 8);
        packet[3] = (uint8_t)(normalWakeupSettlingTime >> 16);
        packet[4] = (uint8_t)(normalWakeupSettlingTime >> 24);

        packet[5] = (uint8_t)(fastWakeupSettlingTime & 0xFF);
        packet[6] = (uint8_t)(fastWakeupSettlingTime >> 8);
        packet[7] = (uint8_t)(fastWakeupSettlingTime >> 16);
        packet[8] = (uint8_t)(fastWakeupSettlingTime >> 24);

        packet[9]  = fastWakeupEnable;
        packet[10] = xtalBoostGain;
        packet[11] = xtalNormalGain;
        packet[12] = bluetoothTxSlicerTrim;
        packet[13] = bluetoothIdleSlicerTrim;
        packet[14] = fastClockInputAcDc;
        packet[15] = slowClockAccuracy;
        packet[16] = clockSource;
        packet[17] = gcmExtraSettlingTime;                    
        packet[18] = reserved;
    }        
    packet -= (HCI_CMD_HDR_LEN);
    hciCmdSend(packet);
}

void vs_cmd_ti_ll_pars(
    uint16_t   inactivityTimeout,
    uint16_t   retransmitTimeout,
    uint8_t    rtsPulseWidth)
{
    uint8_t*   pPacket;

    pPacket = hciCmdAlloc(HCID_CC_TI_HCILL_PARS_CFG, 5);  
    if(pPacket){
        pPacket += (HCI_CMD_HDR_LEN);
        pPacket[0] = (uint8_t)(inactivityTimeout & 0xFF);
        pPacket[1] = (uint8_t)(inactivityTimeout >> 8);
        pPacket[2] = (uint8_t)(retransmitTimeout & 0xFF);
        pPacket[3] = (uint8_t)(retransmitTimeout >> 8);
        pPacket[4] = rtsPulseWidth;
    }
    pPacket -= (HCI_CMD_HDR_LEN);
    hciCmdSend(pPacket);
}

void vs_cmd_sleep_prot_config(
    uint8_t    bigSleepEnable,
    uint8_t    deepSleepEnable,
    uint8_t    deepSleepProtMode,
    uint8_t    outputIoSelect,
    uint8_t    outputPullEnable,
    uint8_t    inputPullEnable,
    uint8_t    inputIoSelect,
    uint16_t   hostWakeDeassertionTimer)
{
    uint8_t*   pPacket;

    pPacket = hciCmdAlloc(HCID_CC_TI_SLEEP_PROTOCOLS_CFG, 9);  
    if(pPacket){
        pPacket += (HCI_CMD_HDR_LEN);
        pPacket[0] = bigSleepEnable;
        pPacket[1] = deepSleepEnable;
        pPacket[2] = deepSleepProtMode;
        pPacket[3] = outputIoSelect;
        pPacket[4] = outputPullEnable;
        pPacket[5] = inputPullEnable;
        pPacket[6] = inputIoSelect;
        pPacket[7] = (uint8_t)(hostWakeDeassertionTimer & 0xFF);
        pPacket[8] = (uint8_t)(hostWakeDeassertionTimer >> 8);
    }
    pPacket -= (HCI_CMD_HDR_LEN);
    hciCmdSend(pPacket);
}
#endif
