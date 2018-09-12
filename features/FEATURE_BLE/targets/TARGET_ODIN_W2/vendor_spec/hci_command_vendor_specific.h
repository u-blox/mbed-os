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

#include "stack_config.h"
#include "hci_defines.h"
#include "hci_defines_vendor_specific.h"

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

#if (SUPPORT_TXTEST == 1)
extern void HCI_CmdEricssonTxTest(
               uint8 ucRxOnStart,
               uint8 ucSyntOnStart,
               uint8 ucTxOnStart,
               uint8 ucPhdOffStart,
               uint8 eTestScenario,
               uint8 eHoppingMode,
               uint8 ucTxFrequency,
               uint8 ucRxFrequency,
               uint8 ucTxTestInterval,
               uint8 ucTestPacketType,
               uint16 uiLengthOfTestData);
#endif /* (SUPPORT_TXTEST == 1) */

#if (HCI_VENDOR_SPECIFIC_SUPPORT_TI == 1)

extern void HCI_CmdTiUpdateUartBaudrate(uint32 baudrate);

#define HCI_CmdTiWriteBdAddress(pTBdAddr) \
    HCI_CmdWithBdAddress(HCID_CC_TI_WRITE_BD_ADDR, pTBdAddr)

#define HCI_CmdTiSystemStatus() \
    HCI_CmdSimple(HCID_CC_TI_SYSTEM_STATUS)

extern void HCI_CmdTiSpiConfiguration();

#define HCI_CmdTiBleOutputPower(powerStep) \
  HCI_CmdWithByte(HCID_CC_TI_BLE_OUTPUT_POWER, powerStep)

#define HCI_CmdTiContinuousRx(channel, adpllLoopMode) \
    HCI_CmdWithByteAndByte(HCID_CC_TI_CONTINUOUS_RX, channel, adpllLoopMode)

extern void HCI_CmdTiContinuousTx(
    cb_uint8 modulationScheme,
    cb_uint8 testPattern,
    cb_uint8 channel,
    cb_uint8 powerLevel,
    cb_uint32 generatorInit,
    cb_uint32 edrGeneratorInit);

extern void HCI_CmdTiPacketTxRx(
    cb_uint8 frequencyMode,
    cb_uint8 txSingleFrequency,
    cb_uint8 rxSingleFrequency,
    cb_uint8 aclPacketType,
    cb_uint8 aclDataPattern,
    cb_uint8 useExtendedFeatures,
    cb_uint16 aclDataLength,
    cb_uint8 powerLevel,    
    cb_uint8 disableWhitening,
    cb_uint16 prbs9Init);

extern void HCI_CmdTiTesterContinousTx(
    cb_uint16 frequency,
    cb_uint8 modulationScheme,
    cb_uint8 testPattern,
    cb_uint8 powerLevelIndex,
    cb_uint32 reserved1,
    cb_uint32 reserved2);

extern void HCI_CmdTiTesterContinousRx(
    cb_uint16 frequency,
    cb_uint8 rxMode,
    cb_uint8 modulationScheme);

extern void HCI_CmdTiTesterPacketTxRx(
    cb_uint8 aclPacketType,
    cb_uint8 frequencyMode,
    cb_uint16 txSingleFrequency,
    cb_uint16 rxSingleFrequency,
    cb_uint8 aclDataPattern,
    cb_uint8 useExtendedFeatures,
    cb_uint16 aclDataLength,
    cb_uint8 powerLevelIndex,
    cb_uint8 disableWhitening,
    cb_uint16 prbs9Init);

extern void HCI_CmdTiSleepProtCfgs(
    cb_uint8    bigSleepEnable,
    cb_uint8    deepSleepEnable,
    cb_uint8    deepSleepProtMode,
    cb_uint8    outputIoSelect,
    cb_uint8    outputPullEnable,
    cb_uint8    inputPullEnable,
    cb_uint8    inputIoSelect,
    cb_uint16   hostWakeDeassertionTimer);

extern void HCI_CmdTiFastClockConfigBtip(
    cb_uint8   xtalEnableStatus,
    cb_uint32  normalWakeupSettlingTime,
    cb_uint32  fastWakeupSettlingTime,
    cb_uint8   fastWakeupEnable,
    cb_uint8   xtalBoostGain,
    cb_uint8   xtalNormalGain,
    cb_uint8   bluetoothTxSlicerTrim,
    cb_uint8   bluetoothIdleSlicerTrim,
    cb_uint8   fastClockInputAcDc,
    cb_uint8   slowClockAccuracy,
    cb_uint8   clockSource,
    cb_uint8   gcmExtraSettlingTime,
    cb_uint8   reserved);

extern void HCI_CmdTiHcillPars(
    cb_uint16   inactivityTimeout,
    cb_uint16   retransmitTimeout,
    cb_uint8    rtsPulseWidth);

#define HCI_CmdTiSetQoSInterval(connHandle, interval) \
        HCI_CmdWithShortAndByte(HCID_CC_TI_SET_QOS_INTERVAL, connHandle, interval)

extern void HCI_CmdTiSetCalibrationInfo(
                                        cb_uint8  splitMode,
                                        cb_uint32 calibrationsWithPaLna,
                                        cb_uint32 calibrationsWithProtect,
                                        cb_uint8  paSdDelay,
                                        cb_uint8  nbrOfCalibrationsPaSdAttempts,
                                        cb_uint8  interferenceDelay,
                                        cb_uint8  nbrOfCalibrationsInterfAttempts,
                                        cb_uint8* pCalibrations);

extern void HCI_CmdTiEnableCalibration(
    cb_uint8  mode,
    cb_uint32 procedure,
    cb_uint8  overrideTempCondition);

extern void HCI_CmdTiLeSetTestModeParams(cb_uint8 txPowerLevelIndex,
                                         cb_uint8 rxMode,
                                         cb_uint16 packetsToTransmit,
                                         cb_uint32 accessCode,
                                         cb_uint8 bleBerTestEnable,
                                         cb_uint8 bleBerTestPattern,
                                         cb_uint8 berTestPacketLength,
                                         cb_uint8 berFaThreshold,
                                         cb_uint8 traceEnable,
                                         cb_uint32 referenceCrc);


extern void HCI_CmdTiConfigDbgMsgOverHci(cb_uint16 mode, cb_uint16 timeout);

extern void HCI_CmdTiEnableProtocolViewer(cb_uint8 enable);


#endif
