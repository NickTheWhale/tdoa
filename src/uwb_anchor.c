#include "uwb.h"

#include "deca_device_api.h"
#include "deca_regs.h"
#include "deca_spi.h"
#include "mac.h"
#include "port.h"

#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/random/rand32.h>

LOG_MODULE_REGISTER(anchor, LOG_LEVEL_DBG);

static uwb_config_t *uwb_config;

static mac_packet_t tx_packet;
static mac_packet_t rx_packet;
typedef struct __packed
{
    uint32_t anchor_x_pos_mm;
    uint32_t anchor_y_pos_mm;
} anchor_payload_t;

static anchor_payload_t *payload = (anchor_payload_t *)&tx_packet.payload;

static struct
{
    uint32_t next_tx_tick;
} ctx;

static void handle_rx_packet();
static uint32_t start_next_event(uint64_t current_ticks);
static uint32_t randomize_delay_to_next_tx();
static int send_tx_packet();
static void anchor_init(uwb_config_t *config);
static uint32_t anchor_on_event(uwb_event_t event);

static void handle_rx_packet()
{
    uint32_t read_size = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFLEN_MASK;
    if (read_size > MAC80215_PACKET_SIZE)
        read_size = MAC80215_PACKET_SIZE;

    dwt_readrxdata((uint8_t *)&rx_packet, read_size, 0);

    MAC80215_LOG_PACKET(&rx_packet);
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
    const uint32_t average_delay = 5000;
    const uint32_t interval = 100;
    uint32_t random = sys_rand32_get();

    uint32_t delay = average_delay + random % interval - interval / 2;
    return delay;
}

static int send_tx_packet()
{
    if (dwt_writetxdata(sizeof(tx_packet), (uint8_t *)&tx_packet, 0) != DWT_SUCCESS)
    {
        return -1;
    }
    dwt_writetxfctrl(sizeof(tx_packet), 0, 1);

    if (dwt_starttx(DWT_START_TX_IMMEDIATE) != DWT_SUCCESS)
    {
        return -2;
    }

    LOG_DBG("tx packet being sent:");
    MAC80215_LOG_PACKET(&tx_packet);

    return 0;
}

static void anchor_init(uwb_config_t *config)
{
    LOG_DBG("Anchor init");
    uwb_config = config;

    MAC80215_PACKET_INIT(&tx_packet, MAC802154_TYPE_DATA);
    memcpy(tx_packet.src_address, uwb_config->address, 8);
    payload->anchor_x_pos_mm = uwb_config->anchor_x_pos_mm;
    payload->anchor_y_pos_mm = uwb_config->anchor_y_pos_mm;

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