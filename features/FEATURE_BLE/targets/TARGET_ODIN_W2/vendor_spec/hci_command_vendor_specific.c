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
#define __CB_FILE__ "hci_command_vendor_specific.c"

#include <string.h>
#include "hci_defines_vendor_specific.h"

#define CB_FILE_CODE CB_FILE_HCI_COMMAND_VENDOR_SPECIFIC

/*=============================================================================
 * INTERFACE ROUTINES
 *=============================================================================
 */

 // A wrapper function it can be made inline to avoid extra function call overhead or it can be macro
uint8_t   *HciCmdAlloc(uint16_t commandCode, uint8_t  parameterLength, uint8_t *pHciPacket)
{
    pHciPacket = hciCmdAlloc(commandCode, parameterLength);
    pHciPacket += HCI_CMD_HDR_LEN;
    return pHciPacket;
}


#if (HCI_VENDOR_SPECIFIC_SUPPORT_TI == 1)
/****************************************************************************/
extern void
HCI_CmdTiUpdateUartBaudrate(uint32 baudrate)
{
    uint8_t      *pPacket;
    uint8_t      *pParameters;

    pParameters = HciCmdAlloc(HCID_CC_TI_UPDATE_UART_BAUD_RATE, 4, pPacket);  
    pParameters[0] = (uint8_t)(baudrate & 0xFF);
    pParameters[1] = (uint8_t)(baudrate >> 8);
    pParameters[2] = (uint8_t)(baudrate >> 16);
    pParameters[3] = (uint8_t)(baudrate >> 24);
    hciCmdSend(pPacket);
}


extern void HCI_CmdTiSpiConfiguration(void)
{
    uint8_t      *pPacket;
    uint8_t      *pParameters;
 

    pParameters = HciCmdAlloc(HCID_CC_TI_SPI_CONFIGURATION, 17, pPacket);  
    
    pParameters[0] = 0x00; //MSB first
    pParameters[1] = 0x01; //
    pParameters[2] = 0x00;
    pParameters[3] = 0x00;
    pParameters[4] = 0x00;
    pParameters[5] = 0x00;
    pParameters[6] = 0x01;
    pParameters[7] = 0x00;
    pParameters[8] = 0x00;
    pParameters[9] = 0x00;
    pParameters[10] = 0x00;
    pParameters[11] = 0x00;
    pParameters[12] = 0x00;
    pParameters[13] = 0x00;
    pParameters[14] = 0x00;
    pParameters[15] = 0x00;
    pParameters[16] = 0x00;

    hciCmdSend(pPacket);
}

extern void HCI_CmdTiContinuousTx(
                                  cb_uint8_t modulationScheme,
                                  cb_uint8_t testPattern,
                                  cb_uint8_t channel,
                                  cb_uint8_t powerLevel,
                                  cb_uint32 generatorInit,
                                  cb_uint32 edrGeneratorInit)
{
    uint8_t      *pPacket;
    uint8_t      *pParameters;

    pParameters = HciCmdAlloc(HCID_CC_TI_CONTINUOUS_TX, 12, pPacket);  
    
    pParameters[0] = modulationScheme;
    pParameters[1] = testPattern;
    pParameters[2] = channel;
    pParameters[3] = powerLevel;

    pParameters[4] = (uint8_t)(generatorInit & 0xFF);
    pParameters[5] = (uint8_t)(generatorInit >> 8);
    pParameters[6] = (uint8_t)(generatorInit >> 16);
    pParameters[7] = (uint8_t)(generatorInit >> 24);

    pParameters[8] = (uint8_t)(edrGeneratorInit & 0xFF);
    pParameters[9] = (uint8_t)(edrGeneratorInit >> 8);
    pParameters[10] = (uint8_t)(edrGeneratorInit >> 16);
    pParameters[11] = (uint8_t)(edrGeneratorInit >> 24);

    hciCmdSend(pPacket);
}


extern void HCI_CmdTiPacketTxRx(
    cb_uint8_t frequencyMode,
    cb_uint8_t txSingleFrequency,
    cb_uint8_t rxSingleFrequency,
    cb_uint8_t aclPacketType,
    cb_uint8_t aclDataPattern,
    cb_uint8_t useExtendedFeatures,
    cb_uint16 aclDataLength,
    cb_uint8_t powerLevel,
    cb_uint8_t disableWhitening,
    cb_uint16 prbs9Init)
{
    uint8_t      *pPacket;
    uint8_t      *pParameters;

    pParameters = HciCmdAlloc(HCID_CC_TI_PACKET_TX_RX, 12, pPacket);  

    pParameters[0] = frequencyMode;
    pParameters[1] = txSingleFrequency;
    pParameters[2] = rxSingleFrequency;
    pParameters[3] = aclPacketType;
    pParameters[4] = aclDataPattern;
    pParameters[5] = useExtendedFeatures;
    pParameters[6] = (uint8_t)(aclDataLength & 0xFF);
    pParameters[7] = (uint8_t)(aclDataLength >> 8);
    pParameters[8] = powerLevel;
    pParameters[9] = disableWhitening;
    pParameters[10] = (uint8_t)(prbs9Init & 0xFF);
    pParameters[11] = (uint8_t)(prbs9Init >> 8);

    hciCmdSend(pPacket);
}

extern void HCI_CmdTiTesterContinousTx(
    cb_uint16 frequency,
    cb_uint8_t modulationScheme,
    cb_uint8_t testPattern,
    cb_uint8_t powerLevelIndex,
    cb_uint32 reserved1,
    cb_uint32 reserved2)
{
    uint8_t      *pPacket;
    uint8_t      *pParameters;

    pParameters = HciCmdAlloc(HCID_CC_TI_DRPb_Tester_Con_TX, 13, pPacket);  
    
    pParameters[0] = (uint8_t)(frequency & 0xFF);
    pParameters[1] = (uint8_t)(frequency >> 8);

    pParameters[2] = modulationScheme;
    pParameters[3] = testPattern;
    pParameters[4] = powerLevelIndex;

    pParameters[5] = (uint8_t)(reserved1 & 0xFF);
    pParameters[6] = (uint8_t)(reserved1 >> 8);
    pParameters[7] = (uint8_t)(reserved1 >> 16);
    pParameters[8] = (uint8_t)(reserved1 >> 24);

    pParameters[9] = (uint8_t)(reserved2 & 0xFF);
    pParameters[10] = (uint8_t)(reserved2 >> 8);
    pParameters[11] = (uint8_t)(reserved2 >> 16);
    pParameters[12] = (uint8_t)(reserved2 >> 24);

    hciCmdSend(pPacket);
}

extern void HCI_CmdTiTesterContinousRx(
    cb_uint16 frequency,
    cb_uint8_t rxMode,
    cb_uint8_t modulationScheme)
{
    uint8_t      *pPacket;
    uint8_t      *pParameters;

    pParameters = HciCmdAlloc(HCID_CC_TI_DRPb_Tester_Con_RX, 4, pPacket);  

    pParameters[0] = (uint8_t)(frequency & 0xFF);
    pParameters[1] = (uint8_t)(frequency >> 8);

    pParameters[2] = rxMode;
    pParameters[3] = modulationScheme;

    hciCmdSend(pPacket);
}


extern void HCI_CmdTiTesterPacketTxRx(
    cb_uint8_t aclPacketType,
    cb_uint8_t frequencyMode,
    cb_uint16 txSingleFrequency,
    cb_uint16 rxSingleFrequency,
    cb_uint8_t aclDataPattern,
    cb_uint8_t reserved,
    cb_uint16 aclDataLength,
    cb_uint8_t powerLevel,
    cb_uint8_t disableWhitening,
    cb_uint16 prbs9Init)
{
    uint8_t      *pPacket;
    uint8_t      *pParameters;

    pParameters = HciCmdAlloc(HCID_CC_TI_DRPb_Tester_Packet_TX_RX, 14, pPacket);

    pParameters[0] = aclPacketType;
    pParameters[1] = frequencyMode;
    pParameters[2] = (cb_uint8_t)(txSingleFrequency & 0xFF);
    pParameters[3] = (cb_uint8_t)(txSingleFrequency >>8);
    pParameters[4] = (cb_uint8_t)(rxSingleFrequency & 0xFF);
    pParameters[5] = (cb_uint8_t)(rxSingleFrequency >>8);
    pParameters[6] = aclDataPattern;
    pParameters[7] = reserved;
    pParameters[8] = (uint8_t)(aclDataLength & 0xFF);
    pParameters[9] = (uint8_t)(aclDataLength >> 8);
    pParameters[10] = powerLevel;
    pParameters[11] = disableWhitening;
    pParameters[12] = (uint8_t)(prbs9Init & 0xFF);
    pParameters[13] = (uint8_t)(prbs9Init >> 8);

    hciCmdSend(pPacket);
}

extern void HCI_CmdTiSleepProtCfgs(
    cb_uint8_t    bigSleepEnable,
    cb_uint8_t    deepSleepEnable,
    cb_uint8_t    deepSleepProtMode,
    cb_uint8_t    outputIoSelect,
    cb_uint8_t    outputPullEnable,
    cb_uint8_t    inputPullEnable,
    cb_uint8_t    inputIoSelect,
    cb_uint16   hostWakeDeassertionTimer)
{
    uint8_t      *pPacket;
    uint8_t      *pParameters;

    pParameters = HciCmdAlloc(HCID_CC_TI_SLEEP_PROTOCOLS_CFG, 9, pPacket);  

    pParameters[0] = bigSleepEnable;
    pParameters[1] = deepSleepEnable;
    pParameters[2] = deepSleepProtMode;
    pParameters[3] = outputIoSelect;
    pParameters[4] = outputPullEnable;
    pParameters[5] = inputPullEnable;
    pParameters[6] = inputIoSelect;
    pParameters[7] = (cb_uint8_t)(hostWakeDeassertionTimer & 0xFF);
    pParameters[8] = (cb_uint8_t)(hostWakeDeassertionTimer >> 8);

    hciCmdSend(pPacket);
}

extern void HCI_CmdTiFastClockConfigBtip(
  cb_uint8_t xtalEnableStatus,
  cb_uint32 normalWakeupSettlingTime,
  cb_uint32 fastWakeupSettlingTime,
  cb_uint8_t fastWakeupEnable,
  cb_uint8_t xtalBoostGain,
  cb_uint8_t xtalNormalGain,
  cb_uint8_t bluetoothTxSlicerTrim,
  cb_uint8_t bluetoothIdleSlicerTrim,
  cb_uint8_t fastClockInputAcDc,
  cb_uint8_t slowClockAccuracy,
  cb_uint8_t clockSource,
  cb_uint8_t gcmExtraSettlingTime,
  cb_uint8_t reserved)                        
{
  uint8_t      *pPacket;
  uint8_t      *pParameters;

  pParameters = HciCmdAlloc(HCID_CC_TI_FAST_CLOCK_CONFIG_BTIP, 19, pPacket);  

  pParameters[0] = xtalEnableStatus;
  
  pParameters[1] = (cb_uint8_t)(normalWakeupSettlingTime & 0xFF);
  pParameters[2] = (cb_uint8_t)(normalWakeupSettlingTime >> 8);
  pParameters[3] = (cb_uint8_t)(normalWakeupSettlingTime >> 16);
  pParameters[4] = (cb_uint8_t)(normalWakeupSettlingTime >> 24);
  
  pParameters[5] = (cb_uint8_t)(fastWakeupSettlingTime & 0xFF);
  pParameters[6] = (cb_uint8_t)(fastWakeupSettlingTime >> 8);
  pParameters[7] = (cb_uint8_t)(fastWakeupSettlingTime >> 16);
  pParameters[8] = (cb_uint8_t)(fastWakeupSettlingTime >> 24);

  pParameters[9]  =  fastWakeupEnable;
  pParameters[10] = xtalBoostGain;
  pParameters[11] = xtalNormalGain;
  pParameters[12] = bluetoothTxSlicerTrim;
  pParameters[13] = bluetoothIdleSlicerTrim;
  pParameters[14] = fastClockInputAcDc;
  pParameters[15] = slowClockAccuracy;
  pParameters[16] = clockSource;
  pParameters[17] = gcmExtraSettlingTime;                    
  pParameters[18] = reserved;

  hciCmdSend(pPacket);
}

extern void HCI_CmdTiEnableCalibration(
    cb_uint8_t  mode,
    cb_uint32 procedure,
    cb_uint8_t  overrideTempCondition)
{
  uint8_t      *pPacket;
  uint8_t      *pParameters;

  pParameters = HciCmdAlloc(HCID_CC_TI_ENABLE_RF_CALIBRATION, 6, pPacket);  

  pParameters[0] = mode;

  pParameters[1] = (uint8_t)(procedure & 0xFF);
  pParameters[2] = (uint8_t)(procedure >> 8);
  pParameters[3] = (uint8_t)(procedure >> 16);
  pParameters[4] = (uint8_t)(procedure >> 24);

  pParameters[5] = (uint8_t)overrideTempCondition;

  hciCmdSend(pPacket);
}

extern void HCI_CmdTiSetCalibrationInfo(
                                       cb_uint8_t  splitMode,
                                       cb_uint32 calibrationsWithPaLna,
                                       cb_uint32 calibrationsWithProtect,
                                       cb_uint8_t  paSdDelay,
                                       cb_uint8_t  nbrOfCalibrationsPaSdAttempts,
                                       cb_uint8_t  interferenceDelay,
                                       cb_uint8_t  nbrOfCalibrationsInterfAttempts,
                                       cb_uint8_t* pCalibrations)
{
    HCI_Packet pPacket;
    uint8_t      *pParameters;

    pParameters = HciCmdAlloc(HCID_CC_TI_SET_CALIBRATION_INFO, 49, pPacket);  

    pParameters[0] = splitMode;

    pParameters[1] = (uint8_t)(calibrationsWithPaLna & 0xFF);
    pParameters[2] = (uint8_t)(calibrationsWithPaLna >> 8);
    pParameters[3] = (uint8_t)(calibrationsWithPaLna >> 16);
    pParameters[4] = (uint8_t)(calibrationsWithPaLna >> 24);

    pParameters[5] = (uint8_t)(calibrationsWithProtect & 0xFF);
    pParameters[6] = (uint8_t)(calibrationsWithProtect >> 8);
    pParameters[7] = (uint8_t)(calibrationsWithProtect >> 16);
    pParameters[8] = (uint8_t)(calibrationsWithProtect >> 24);

    pParameters[9] = (uint8_t)paSdDelay;
    pParameters[10] = (uint8_t)nbrOfCalibrationsPaSdAttempts;
    pParameters[11] = (uint8_t)interferenceDelay;
    pParameters[12] = (uint8_t)nbrOfCalibrationsInterfAttempts;

    memcpy(&pParameters[13],pCalibrations,32);

    pParameters[45] = 0x00; // reserved
    pParameters[46] = 0x00; // reserved
    pParameters[47] = 0x00; // reserved
    pParameters[48] = 0x00; // reserved

    hciCmdSend(pPacket);
}

extern void HCI_CmdTiHcillPars(
    cb_uint16   inactivityTimeout,
    cb_uint16   retransmitTimeout,
    cb_uint8_t    rtsPulseWidth)
{
    uint8_t      *pPacket;
    uint8_t      *pParameters;

    pParameters = HciCmdAlloc(HCID_CC_TI_HCILL_PARS_CFG, 5, pPacket);  

    pParameters[0] = (cb_uint8_t)(inactivityTimeout & 0xFF);
    pParameters[1] = (cb_uint8_t)(inactivityTimeout >> 8);
    pParameters[2] = (cb_uint8_t)(retransmitTimeout & 0xFF);
    pParameters[3] = (cb_uint8_t)(retransmitTimeout >> 8);
    pParameters[4] = rtsPulseWidth;

    hciCmdSend(pPacket);
}

extern void HCI_CmdTiLeSetTestModeParams(cb_uint8_t txPowerLevelIndex,
                                         cb_uint8_t rxMode,
                                         cb_uint16 packetsToTransmit,
                                         cb_uint32 accessCode,
                                         cb_uint8_t bleBerTestEnable,
                                         cb_uint8_t bleBerTestPattern,
                                         cb_uint8_t berTestPacketLength,
                                         cb_uint8_t berFaThreshold,
                                         cb_uint8_t traceEnable,
                                         cb_uint32 referenceCrc)
{
    uint8_t      *pPacket;
    uint8_t      *pParameters;

    pParameters = HciCmdAlloc(HCID_CC_TI_SET_LE_TEST_MODE_PARAMS, 17, pPacket);  

    pParameters[0] = txPowerLevelIndex;
    pParameters[1] = rxMode;
    pParameters[2] = (uint8_t)(packetsToTransmit & 0xFF);;
    pParameters[3] = (uint8_t)(packetsToTransmit >> 8);

    pParameters[4] = (uint8_t)(accessCode & 0xFF);
    pParameters[5] = (uint8_t)(accessCode >> 8);
    pParameters[6] = (uint8_t)(accessCode >> 16);
    pParameters[7] = (uint8_t)(accessCode >> 24);
    pParameters[8] = bleBerTestEnable;
    pParameters[9] = bleBerTestPattern;
    pParameters[10] = berTestPacketLength;
    pParameters[11] = berFaThreshold;
    pParameters[12] = traceEnable;
    pParameters[13] = (uint8_t)(referenceCrc & 0xFF);
    pParameters[14] = (uint8_t)(referenceCrc >> 8);
    pParameters[15] = (uint8_t)(referenceCrc >> 16);
    pParameters[16] = (uint8_t)(referenceCrc >> 24);

    hciCmdSend(pPacket);
}

extern void
HCI_CmdTiConfigDbgMsgOverHci(cb_uint16 mode, cb_uint16 timeout)
{
    uint8_t      *pPacket;
    uint8_t      *pParameters;

    pParameters = HciCmdAlloc(HCID_CC_TI_CONFIG_DBG_MSG_OVER_HCI, 4, pPacket);  
    pParameters[0] = (uint8_t)(mode & 0xFF);
    pParameters[1] = (uint8_t)(mode >> 8);
    pParameters[2] = (uint8_t)(timeout & 0xFF);
    pParameters[3] = (uint8_t)(timeout >> 8);
    hciCmdSend(pPacket);
}

extern void
HCI_CmdTiEnableProtocolViewer(cb_uint8_t enable)
{
    uint8_t      *pPacket;
    uint8_t      *pParameters;

    pParameters = HciCmdAlloc(HCID_CC_TI_ENABLE_PROTOCOL_VIEWER, 1, pPacket);  
    pParameters[0] = enable;
    hciCmdSend(pPacket);
}

#endif
