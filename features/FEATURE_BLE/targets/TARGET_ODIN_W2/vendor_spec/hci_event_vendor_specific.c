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
#define __CB_FILE__ "hci_event_vendor_specific.c"

#include <string.h>
#include "hci_event_vendor_specific.h"

/*=============================================================================
 * LOCAL DEFINED CONSTANTS & MACROS
 *=============================================================================
 */

#define CB_FILE_CODE CB_FILE_HCI_EVENT_VENDOR_SPECIFIC

/*=============================================================================
 * LOCAL TYPES
 *=============================================================================
 */

/*=============================================================================
 * LOCAL (STATIC) ROUTINES PROTOTYPES
 *=============================================================================
 */

/*=============================================================================
 * LOCAL (STATIC) DATA
 *=============================================================================
 */

static HCIE_TVendorSpecCallBack *hciVendorSpecCallback;
static HCIE_TVendorSpecInitCallBack *hciVendorSpecInitCallback;

/*=============================================================================
 * LOCAL (STATIC) ROUTINES
 *=============================================================================
 */


/*=============================================================================
 * INTERFACE ROUTINES
 *=============================================================================
 */

extern void HCIE_RegisterVendorSpecCallBack(const HCIE_TVendorSpecCallBack *ptCallBack)
{
  hciVendorSpecCallback = ptCallBack;
}

extern void HCIE_RegisterVendorSpecInitCallBack(HCIE_TVendorSpecInitCallBack *ptCallBack)
{
  hciVendorSpecInitCallback = ptCallBack;
}

void HCIE_HandleVendorSpecEvt(uint8_t *pParameter, uint8_t length)
{
#ifdef CB_DEBUG_FWD_TI_ISLAND
    uint8_t hdr[] = {0x04,0xFF,length};
    cbDEBUG_UART_write(hdr,3);
    cbDEBUG_UART_write(pParameter,length);
#endif
  /* Ignore - No vendor specific events are yet implemented. */
}

void HCIE_HandleVendorSpecCmdCmplEvt(uint8_t *pParameters)
{
  uint8_t   status;
  uint16_t commandCode = pParameters[1] | (pParameters[2] << 8);
  status =  pParameters[3];

  switch (commandCode)
  {
#if (HCI_VENDOR_SPECIFIC_SUPPORT_TI == 1)
  case HCID_VENDOR_SPECIFIC_COMMANDS | HCID_CC_TI_CONTINUOUS_RX:
  case HCID_VENDOR_SPECIFIC_COMMANDS | HCID_CC_TI_CONTINUOUS_TX:
  case HCID_VENDOR_SPECIFIC_COMMANDS | HCID_CC_TI_PACKET_TX_RX:
  case HCID_VENDOR_SPECIFIC_COMMANDS | HCID_CC_TI_DRPb_Tester_Packet_TX_RX:
  case HCID_VENDOR_SPECIFIC_COMMANDS | HCID_CC_TI_DRPb_Tester_Con_TX:
  case HCID_VENDOR_SPECIFIC_COMMANDS | HCID_CC_TI_DRPb_Tester_Con_RX:
      hciVendorSpecCallback->pfTestCmdStartEvt(pParameters[3]);
      break;
  case HCID_VENDOR_SPECIFIC_COMMANDS | HCID_CC_TI_SYSTEM_STATUS:
      hciVendorSpecCallback->pfSytemStatus(status, pParameters[23]);
      break;
#endif
  default:
    break;
  }
  (void)status;
}
