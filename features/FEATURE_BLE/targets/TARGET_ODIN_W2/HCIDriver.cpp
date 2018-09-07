/*
 * Copyright (c) 2018 ARM Limited
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

#include <stdio.h>
#include "CordioBLE.h"
#include "mbed.h"
#include "CordioHCIDriver.h"
#include "hci_api.h"
#include "hci_cmd.h"
#include "hci_core.h"
#include "bstream.h"
#include "wsf_buf.h"
#include "cb_pio.h"
#include <stdbool.h>
#include "hci_mbed_os_adaptation.h"
#include "H4TransportDriver.h"

static const uint8_t service_pack [] = {
	    0x01,0x36,0xff,0x04,0xc0,0xc6,0x2d,0x00
};

#define HCI_RESET_RAND_CNT        4

/**
 * Contain description of the memory pool used by the Cordio stack.
   this is redundant need to be kicked off
 */
struct buf_pool_desc_t {
    /**
     * Create a new memory pool description
     * @param buffer the Buffer used by the memory pool.
     * @param pool_desc How the memory is split
     */
    template<size_t BufferSize, size_t PoolCount>
    buf_pool_desc_t(
        uint8_t (&buffer)[BufferSize],
        const wsfBufPoolDesc_t (&pool_desc)[PoolCount]
    ) : buffer_memory(buffer), buffer_size(BufferSize),
        pool_description(pool_desc), pool_count(PoolCount)
    {
    }

    uint8_t* buffer_memory;         /// Pointer to the buffer memory
    size_t buffer_size;             /// Size of the buffer
    const wsfBufPoolDesc_t* pool_description;   /// Pointer to the first element describing the pool
    size_t pool_count;      /// Number of pools
};

namespace ble {
namespace vendor {
namespace odin_w2 {

class HCIDriver : public cordio::CordioHCIDriver {
public:
    HCIDriver(cordio::CordioHCITransportDriver& transport_driver, PinName shutdown) :
        cordio::CordioHCIDriver(transport_driver),
        shutdown(shutdown, 0),
        //hci_rts(hci_rts, 0),
        service_pack_index(0),
        service_pack_transfered(false) {
    };
    
    virtual void do_initialize();

    virtual void do_terminate();

    virtual void start_reset_sequence();

    virtual void handle_reset_sequence(uint8_t *pMsg);
    
private:
    void start_service_pack_transfert(void) {
        service_pack_index = 0;
        service_pack_transfered = false;
        send_service_pack_command();
    }

    void send_service_pack_command(void) {
        uint16_t cmd_len = service_pack[service_pack_index + HCI_CMD_HDR_LEN];
        uint16_t cmd_opcode = (service_pack[service_pack_index + 2] << 8) | service_pack[service_pack_index + 1];
        uint8_t *pBuf = hciCmdAlloc(cmd_opcode, cmd_len);
        if (pBuf) {
            memcpy(pBuf, service_pack + service_pack_index + 1, cmd_len + HCI_CMD_HDR_LEN);
            hciCmdSend(pBuf);
        } else {
            printf("Error cannot allocate memory for the buffer");
        }
    }

    void ack_service_pack_command(uint16_t opcode, uint8_t* msg) {
        uint16_t cmd_opcode = (service_pack[service_pack_index + 2] << 8) | service_pack[service_pack_index + 1];

        if (cmd_opcode != opcode)  {
            // DO something in case of error
            while (true);
        }

        // update service pack index
        service_pack_index += (1 + HCI_CMD_HDR_LEN + service_pack[service_pack_index + HCI_CMD_HDR_LEN]);

        if (service_pack_index < sizeof(service_pack)) {
            send_service_pack_command();
        } else {
            service_pack_transfered = true;
            /* send an HCI Reset command to start the sequence */
            HciResetCmd();
        }
    }

    void hciCoreReadResolvingListSize(void)
    {
        /* if LL Privacy is supported by Controller and included */
        if ((hciCoreCb.leSupFeat & HCI_LE_SUP_FEAT_PRIVACY) &&
            (hciLeSupFeatCfg & HCI_LE_SUP_FEAT_PRIVACY))
        {
            /* send next command in sequence */
            HciLeReadResolvingListSize();
        }
        else
        {
            hciCoreCb.resListSize = 0;

            /* send next command in sequence */
            hciCoreReadMaxDataLen();
        }
    }

    void hciCoreReadMaxDataLen(void)
    {
    /* if LE Data Packet Length Extensions is supported by Controller and included */
        if ((hciCoreCb.leSupFeat & HCI_LE_SUP_FEAT_DATA_LEN_EXT) &&
            (hciLeSupFeatCfg & HCI_LE_SUP_FEAT_DATA_LEN_EXT))
        {
            /* send next command in sequence */
            HciLeReadMaxDataLen();
        }
        else
        {
            /* send next command in sequence */
            HciLeRandCmd();
        }
    }

    void init_32k() {
        //MXC_PWRSEQ->reg4 |= MXC_F_PWRSEQ_REG4_PWR_PSEQ_32K_EN;
    }

    DigitalOut shutdown;
    //DigitalOut hci_rts;
    size_t service_pack_index;
    bool service_pack_transfered;
};

} // namespace odin_w2
} // namespace vendor
} // namespace ble



extern "C" void cbPIO_open(
        cbPIO_Pin               pin,
        cbPIO_Mode              mode,
        cbPIO_Value             value);

extern "C" void cbPIO_write(cbPIO_Pin       pin,
        cbPIO_Value     value);

void ble::vendor::odin_w2::HCIDriver::do_initialize()
{
    //GPIO_InitTypeDef cfg;

    //cfg.Pin = shutdown;
    //cfg.Speed = GPIO_SPEED_FAST;
    //cfg.Mode = GPIO_MODE_OUTPUT_PP;
    //cfg.Pull = GPIO_NOPULL;

    shutdown = 0;
    //wait_ms(100);
    // init_32k();
    //wait_ms(20);
    //shutdown = 1;
    //hci_rts = 0;
    //wait_ms(500);
    cbPIO_open(cbPIO_PIN_BT_ENABLE, cbPIO_MODE_OUTPUT, cbPIO_VALUE_LOW);

    // Wait and enable BT
    wait_ms(20);//cbOS_delay(20000); //20ms
    cbPIO_write(cbPIO_PIN_BT_ENABLE, cbPIO_VALUE_HIGH);
    wait_ms(500);//cbOS_delay(500000); //500ms
}

void ble::vendor::odin_w2::HCIDriver::do_terminate()
{   
}

void ble::vendor::odin_w2::HCIDriver::start_reset_sequence()
{
    start_service_pack_transfert();
}

void ble::vendor::odin_w2::HCIDriver::handle_reset_sequence(uint8_t *pMsg)
{
    uint16_t       opcode;
    static uint8_t randCnt;

    /* if event is a command complete event */
    if (*pMsg == HCI_CMD_CMPL_EVT)
    {
        /* parse parameters */
        pMsg += HCI_EVT_HDR_LEN;
        pMsg++;                   /* skip num packets */
        BSTREAM_TO_UINT16(opcode, pMsg);
        pMsg++;                   /* skip status */

        if (service_pack_transfered == false) {
            ack_service_pack_command(opcode, pMsg);
            return;
        }

        /* decode opcode */
        switch (opcode)
        {
            case HCI_OPCODE_RESET:
                /* initialize rand command count */
                randCnt = 0;

                /* send next command in sequence */
                HciSetEventMaskCmd((uint8_t *) hciEventMask);
                break;

            case HCI_OPCODE_SET_EVENT_MASK:
                /* send next command in sequence */
                HciLeSetEventMaskCmd((uint8_t *) hciLeEventMask);
                break;

            case HCI_OPCODE_LE_SET_EVENT_MASK:
                /* send next command in sequence */
                HciSetEventMaskPage2Cmd((uint8_t *) hciEventMaskPage2);
                break;

            case HCI_OPCODE_SET_EVENT_MASK_PAGE2:
                /* send next command in sequence */
                HciReadBdAddrCmd();
                break;

            case HCI_OPCODE_READ_BD_ADDR:
                /* parse and store event parameters */
                BdaCpy(hciCoreCb.bdAddr, pMsg);

                /* send next command in sequence */
                HciLeReadBufSizeCmd();
                break;

            case HCI_OPCODE_LE_READ_BUF_SIZE:
                /* parse and store event parameters */
                BSTREAM_TO_UINT16(hciCoreCb.bufSize, pMsg);
                BSTREAM_TO_UINT8(hciCoreCb.numBufs, pMsg);

                /* initialize ACL buffer accounting */
                hciCoreCb.availBufs = hciCoreCb.numBufs;

                /* send next command in sequence */
                HciLeReadSupStatesCmd();
                break;

            case HCI_OPCODE_LE_READ_SUP_STATES:
                /* parse and store event parameters */
                memcpy(hciCoreCb.leStates, pMsg, HCI_LE_STATES_LEN);

                /* send next command in sequence */
                HciLeReadWhiteListSizeCmd();
                break;

            case HCI_OPCODE_LE_READ_WHITE_LIST_SIZE:
                /* parse and store event parameters */
                BSTREAM_TO_UINT8(hciCoreCb.whiteListSize, pMsg);

                /* send next command in sequence */
                HciLeReadLocalSupFeatCmd();
                break;

            case HCI_OPCODE_LE_READ_LOCAL_SUP_FEAT:
                /* parse and store event parameters */
                BSTREAM_TO_UINT16(hciCoreCb.leSupFeat, pMsg);

                /* send next command in sequence */
                hciCoreReadResolvingListSize();
                break;

            case HCI_OPCODE_LE_READ_RES_LIST_SIZE:
                /* parse and store event parameters */
                BSTREAM_TO_UINT8(hciCoreCb.resListSize, pMsg);

                /* send next command in sequence */
                hciCoreReadMaxDataLen();
                break;

            case HCI_OPCODE_LE_READ_MAX_DATA_LEN:
                {
                    uint16_t maxTxOctets;
                    uint16_t maxTxTime;

                    BSTREAM_TO_UINT16(maxTxOctets, pMsg);
                    BSTREAM_TO_UINT16(maxTxTime, pMsg);

                    /* use Controller's maximum supported payload octets and packet duration times
                    * for transmission as Host's suggested values for maximum transmission number
                    * of payload octets and maximum packet transmission time for new connections.
                    */
                    HciLeWriteDefDataLen(maxTxOctets, maxTxTime);
                }
                break;

            case HCI_OPCODE_LE_WRITE_DEF_DATA_LEN:
                if (hciCoreCb.extResetSeq)
                {
                    /* send first extended command */
                    (*hciCoreCb.extResetSeq)(pMsg, opcode);
                }
                else
                {
                    /* initialize extended parameters */
                    hciCoreCb.maxAdvDataLen = 0;
                    hciCoreCb.numSupAdvSets = 0;
                    hciCoreCb.perAdvListSize = 0;

                    /* send next command in sequence */
                    HciLeRandCmd();
                }
                break;

            case HCI_OPCODE_LE_READ_MAX_ADV_DATA_LEN:
            case HCI_OPCODE_LE_READ_NUM_SUP_ADV_SETS:
            case HCI_OPCODE_LE_READ_PER_ADV_LIST_SIZE:
                if (hciCoreCb.extResetSeq)
                {
                /* send next extended command in sequence */
                (*hciCoreCb.extResetSeq)(pMsg, opcode);
                }
                break;

            case HCI_OPCODE_LE_RAND:
                /* check if need to send second rand command */
                if (randCnt < (HCI_RESET_RAND_CNT-1))
                {
                    randCnt++;
                    HciLeRandCmd();
                }
                else
                {
                    signal_reset_sequence_done();
                }
                break;

            default:
                break;
        }
    }
}

ble::vendor::cordio::CordioHCIDriver& ble_cordio_get_hci_driver() {
    static ble::vendor::cordio::H4TransportDriver transport_driver (
            /* TX */ PG_14, /* RX */ PC_7, /* cts */ PG_15, /* rts */ PG_12, 115200
    );
    static ble::vendor::odin_w2::HCIDriver hci_driver (
            transport_driver, PG_7
    );
    return hci_driver;
}
