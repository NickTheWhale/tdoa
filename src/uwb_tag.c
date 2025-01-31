/**
 * @file uwb_tag.c
 * @author Nicholas Loehrke (nicholasnloehrke@gmail.com)
 * @brief
 * @version 1.0.0
 * @date 2024-03-19
 *
 * MIT License
 *
 * Copyright (c) 2024 Nicholas Loehrke
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "uwb.h"

#include "deca_device_api.h"
#include "deca_regs.h"
#include "deca_spi.h"
#include "mac.h"
#include "port.h"

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(tag, LOG_LEVEL_DBG);

static uwb_config_t *uwb_config;

static mac_packet_t rx_packet;

typedef struct __packed
{
    uint32_t anchor_x_pos_mm;
    uint32_t anchor_y_pos_mm;
} anchor_payload_t;

static void tag_init(uwb_config_t *config);
static uint32_t tag_on_event(uwb_event_t event);

static void tag_init(uwb_config_t *config)
{
    uwb_config = config;
    LOG_DBG("Tag init");
    dwt_rxenable(DWT_START_RX_IMMEDIATE);
}

static uint32_t tag_on_event(uwb_event_t event)
{
    if (event == UWB_EVENT_PACKET_RECEIVED)
    {

        uint32_t read_size = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFLEN_MASK;
        if (read_size > MAC80215_PACKET_SIZE)
            read_size = MAC80215_PACKET_SIZE;

        dwt_readrxdata((uint8_t *)&rx_packet, read_size, 0);
        dwt_rxenable(DWT_START_RX_IMMEDIATE);

        anchor_payload_t *anchor_payload = (anchor_payload_t *)&rx_packet.payload;
        uint8_t *src = rx_packet.src_address;
        LOG_DBG("Anchor '%u:%u:%u:%u:%u:%u:%u:%u' x= %u, y= %u",
                src[0],
                src[1],
                src[2],
                src[3],
                src[4],
                src[5],
                src[6],
                src[7],
                anchor_payload->anchor_x_pos_mm,
                anchor_payload->anchor_y_pos_mm);
    }
    else
    {
        dwt_rxenable(DWT_START_RX_IMMEDIATE);
    }

    return UWB_TIMEOUT_MAXIMUM;
}

uwb_algorithm_t uwb_tag_algorithm = {
    .init = tag_init,
    .on_event = tag_on_event};