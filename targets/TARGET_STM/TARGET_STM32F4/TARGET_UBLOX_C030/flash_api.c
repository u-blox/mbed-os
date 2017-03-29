/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#include "flash_api.h"
#include "flash_data.h"
#include "mbed_critical.h"

// This file is automagically generated

#if DEVICE_FLASH

// This is a flash algo binary blob. It is PIC (position independent code) that should be stored in RAM
static uint32_t FLASH_ALGO[] = {
    0x0e000300, 0xd3022820, 0x1d000940, 0x28104770, 0x0900d302, 0x47701cc0, 0x47700880, 0x49414842,
    0x49426041, 0x21006041, 0x68c16001, 0x431122f0, 0x694060c1, 0xd4060680, 0x493d483e, 0x21066001,
    0x493d6041, 0x20006081, 0x48374770, 0x05426901, 0x61014311, 0x47702000, 0x4833b510, 0x24046901,
    0x61014321, 0x03a26901, 0x61014311, 0x4a314933, 0x6011e000, 0x03db68c3, 0x6901d4fb, 0x610143a1,
    0xbd102000, 0xf7ffb530, 0x4927ffbb, 0x23f068ca, 0x60ca431a, 0x610c2402, 0x0700690a, 0x43020e40,
    0x6908610a, 0x431003e2, 0x48246108, 0xe0004a21, 0x68cd6010, 0xd4fb03ed, 0x43a06908, 0x68c86108,
    0x0f000600, 0x68c8d003, 0x60c84318, 0xbd302001, 0x4d15b570, 0x08891cc9, 0x008968eb, 0x433326f0,
    0x230060eb, 0x4b16612b, 0x692ce017, 0x612c431c, 0x60046814, 0x03e468ec, 0x692cd4fc, 0x00640864,
    0x68ec612c, 0x0f240624, 0x68e8d004, 0x60e84330, 0xbd702001, 0x1d121d00, 0x29001f09, 0x2000d1e5,
    0x0000bd70, 0x45670123, 0x40023c00, 0xcdef89ab, 0x00005555, 0x40003000, 0x00000fff, 0x0000aaaa,
    0x00000201, 0x00000000
};

static const flash_algo_t flash_algo_config = {
    .init = 0x1d,
    .uninit = 0x4b,
    .erase_sector = 0x85,
    .program_page = 0xd1,
    .static_base = 0x144,
    .algo_blob = FLASH_ALGO
};

static const sector_info_t sectors_info[] = {
    {0x8000000, 0x4000},
    {0x8010000, 0x10000},
    {0x8020000, 0x20000},
};

static const flash_target_config_t flash_target_config = {
    .page_size  = 0x400,
    .flash_start = 0x8000000,
    .flash_size = 0x100000,
    .sectors = sectors_info,
    .sector_info_count = sizeof(sectors_info) / sizeof(sector_info_t)
};

void flash_set_target_config(flash_t *obj)
{
    obj->flash_algo = &flash_algo_config;
    obj->target_config = &flash_target_config;
}

#endif