/**
 * @file uwb_anchor.c
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
#include "uwb_utils.h"

#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
// #include <zephyr/random/rand32.h>
#include <zephyr/random/random.h>

LOG_MODULE_REGISTER(anchor, LOG_LEVEL_DBG);

#define TX_SEND_DELAY

static uwb_config_t *uwb_config;

typedef struct __packed
{
    uint64_t sys_time;
    uint32_t anchor_x_pos_mm;
    uint32_t anchor_y_pos_mm;
} anchor_sync_payload_t;

static struct
{
    uint32_t next_tx_tick;
} ctx;

static uint64_t prev_sys_time = 0;

static void handle_rx_packet();
static uint32_t start_next_event(uint64_t current_ticks);
static uint32_t randomize_delay_to_next_tx();
static int send_tx_packet();
static void anchor_init(uwb_config_t *config);
static uint32_t anchor_on_event(uwb_event_t event);

static void handle_rx_packet()
{
    mac_packet_t rx_packet;
    anchor_sync_payload_t *rx_payload = (anchor_sync_payload_t *)&rx_packet.payload;
    uint8_t ts_b[5];
    uint64_t rx_timestamp, tx_timestamp;
    uint32_t read_size;

    dwt_readrxtimestamp(ts_b);
    rx_timestamp = uwb_utils_timestamp_to_u64(ts_b);

    dwt_readtxtimestamp(ts_b);
    tx_timestamp = uwb_utils_timestamp_to_u64(ts_b);

    read_size = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFLEN_MASK;
    if (read_size > MAC80215_PACKET_SIZE)
        read_size = MAC80215_PACKET_SIZE;
    dwt_readrxdata((uint8_t *)&rx_packet, read_size, 0);

    uint64_t sys_time = rx_payload->sys_time;
    int64_t sys_time_delta = sys_time - prev_sys_time;
    prev_sys_time = sys_time;
    int64_t delta = rx_timestamp - tx_timestamp;
    float delta_percent = (double)rx_timestamp / (double)tx_timestamp;

    LOG_RAW("rx: %llu\ntx: %llu\ndelta: %llu\ndelta (%%): %f\nsys_time: %llu\nsys_time_delta: %lld\n\n",
            rx_timestamp,
            tx_timestamp,
            delta,
            delta_percent,
            sys_time,
            sys_time_delta);

    // MAC80215_LOG_PACKET(&rx_packet);
}

static uint32_t start_next_event(uint64_t current_ticks)
{
    dwt_forcetrxoff();

    if (ctx.next_tx_tick < current_ticks)
    {
        uint32_t new_delay = randomize_delay_to_next_tx();
        ctx.next_tx_tick = current_ticks + k_ms_to_ticks_floor64(new_delay);

        send_tx_packet();
    }
    else
    {
        dwt_rxenable(DWT_START_RX_IMMEDIATE);
    }

    uint32_t delay = ctx.next_tx_tick - current_ticks;
    return k_ticks_to_ms_floor32(delay);
}

static uint32_t randomize_delay_to_next_tx()
{
    const uint32_t average_delay = 200;
    const uint32_t interval = 10;
    uint32_t random = sys_rand32_get();

    uint32_t delay = average_delay + random % interval - interval / 2;
    return delay;
}

static int send_tx_packet()
{

    uint8_t ts_b[5];
    dwt_readsystime(ts_b);
    uint64_t start = uwb_utils_timestamp_to_u64(ts_b);

    uint32_t delay = (start + (200000000 * 10)) >> 8;
    dwt_setdelayedtrxtime(delay);

    mac_packet_t tx_packet;
    anchor_sync_payload_t *tx_payload = (anchor_sync_payload_t *)&tx_packet.payload;
    MAC80215_PACKET_INIT(&tx_packet, MAC802154_TYPE_DATA);
    memcpy(tx_packet.src_address, uwb_config->address, 8);
    tx_payload->anchor_x_pos_mm = uwb_config->anchor_x_pos_mm;
    tx_payload->anchor_y_pos_mm = uwb_config->anchor_y_pos_mm;

    if (dwt_writetxdata(sizeof(tx_packet), (uint8_t *)&tx_packet, 0) != DWT_SUCCESS)
    {
        LOG_ERR("Failed to write tx data");
        return -1;
    }
    dwt_writetxfctrl(sizeof(tx_packet), 0, 1);

    if (dwt_starttx(DWT_START_TX_DELAYED) != DWT_SUCCESS)
    {
        LOG_ERR("Failed to send tx packet");
        return -2;
    }

    dwt_readsystime(ts_b);
    uint64_t end = uwb_utils_timestamp_to_u64(ts_b);

    uint64_t send_time = end - start;

    LOG_DBG("Send time: %llu", send_time);

    return 0;
}

static void anchor_init(uwb_config_t *config)
{
    LOG_DBG("Anchor init");
    uwb_config = config;

    ctx.next_tx_tick = 0;
}

static uint32_t anchor_on_event(uwb_event_t event)
{
    if (event == UWB_EVENT_PACKET_RECEIVED)
    {
        handle_rx_packet();
    }

    int64_t now = k_uptime_ticks();
    return start_next_event(now);
}

uwb_algorithm_t uwb_anchor_algorithm = {
    .init = anchor_init,
    .on_event = anchor_on_event};