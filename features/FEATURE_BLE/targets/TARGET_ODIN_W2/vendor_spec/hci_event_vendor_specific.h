#ifndef HCIE_VS_H
#define HCIE_VS_H
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

#include "hci_defines_vendor_specific.h"
#include <stdio.h>

/*==============================================================================
 * CONSTANTS
 *==============================================================================
 */

/*==============================================================================
 * TYPES
 *==============================================================================
 */
typedef uint16_t      TConnHandle;

typedef void (*HCIE_STSetBaudrateEvt)          (uint8_t status);
typedef void (*HCIE_EricssonReadInfoEvt)       (uint8_t status, uint8_t *pRevInfo);
typedef void (*HCIE_EricssonStoreInFlashEvt)   (uint8_t status);
typedef void (*HCIE_EricssonWriteFileBlockEvt) (uint8_t status, uint8_t chunkId);

typedef void (*HCIE_TpfWriteMaxPowerReqConfPos)(TConnHandle ConnectionHandle);  
typedef void (*HCIE_TpfWriteMaxPowerReqConfNeg)(TConnHandle ConnectionHandle);  
typedef void (*HCIE_TpfReadMaxPowerReqConfPos)(TConnHandle ConnectionHandle, uint8_t maxPower);  
typedef void (*HCIE_TpfReadMaxPowerReqConfNeg)(TConnHandle ConnectionHandle);  
typedef void (*HCIE_Dummy) (void);

typedef struct
{          
#if ((HCI_VENDOR_SPECIFIC_SUPPORT_ST == 1) || (HCI_VENDOR_SPECIFIC_SUPPORT_TI == 1)) 
    HCIE_TpfTestCmdStartEvt        pfTestCmdStartEvt;
    HCIE_TISystemStatus            pfSytemStatus;
#endif
  
  HCIE_Dummy                       pfDummy;

} HCIE_TVendorSpecCallBack;

typedef struct
{    
  HCIE_Dummy                       pfDummy;

} HCIE_TVendorSpecInitCallBack;


/*=============================================================================
 * EXPORTED FUNCTIONS
 *=============================================================================
 */

extern void HCIE_RegisterVendorSpecInitCallBack(HCIE_TVendorSpecInitCallBack *ptCallBack);

extern void HCIE_RegisterVendorSpecCallBack(const HCIE_TVendorSpecCallBack *ptCallBack);

#endif

