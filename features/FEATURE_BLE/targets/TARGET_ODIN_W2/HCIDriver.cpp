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
#include "CordioHCIDriver.h"
#include "hci_api.h"
#include "hci_cmd.h"
#include "hci_core.h"
#include "bstream.h"
#include "wsf_buf.h"
#include <stdbool.h>
#include "hci_mbed_os_adaptation.h"
#include "H4TransportDriver.h"

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
namespace wl1837 {

class HCIDriver : public cordio::CordioHCIDriver {
public:
    HCIDriver(
        cordio::CordioHCITransportDriver& transport_driver
    ); 
    
    virtual void do_initialize();

    virtual void do_terminate();

    virtual void start_reset_sequence();

    virtual void handle_reset_sequence(uint8_t *pMsg);
    
private:

    
};

} // namespace wl1837
} // namespace vendor
} // namespace ble

ble::vendor::cordio::CordioHCIDriver& ble_cordio_get_hci_driver() {
    
}

void ble::vendor::wl1837::HCIDriver::do_initialize()
{
    
    
}

void ble::vendor::wl1837::HCIDriver::do_terminate()
{
    
    
}

void ble::vendor::wl1837::HCIDriver::start_reset_sequence()
{
    
    
}

void ble::vendor::wl1837::HCIDriver::handle_reset_sequence(uint8_t *pMsg)
{
    
    
}
