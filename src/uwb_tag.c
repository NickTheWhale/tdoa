#include "uwb.h"

#include "deca_device_api.h"
#include "deca_regs.h"
#include "deca_spi.h"
#include "mac.h"
#include "port.h"

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(tag, LOG_LEVEL_DBG);

static uwb_config_t *uwb_config;

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
        size_t packet_size = sizeof(packet_t);
        packet_t rx_packet;

        uint32_t read_size = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFLEN_MASK;
        if (read_size > packet_size)
            read_size = packet_size;

        dwt_readrxdata((uint8_t *)&rx_packet, read_size, 0);
        dwt_rxenable(DWT_START_RX_IMMEDIATE);

        uint8_t *src = rx_packet.src_address;
        uint8_t *dest = rx_packet.dest_address;

        LOG_PRINTK("Received from %u:%u:%u:%u:%u:%u:%u:%u to %u:%u:%u:%u:%u:%u:%u:%u\n",
                   src[0], src[1], src[2], src[3], src[4], src[5], src[6], src[7],
                   dest[0], dest[1], dest[2], dest[3], dest[4], dest[5], dest[6], dest[7]);
    }
    else
    {
        dwt_rxenable(DWT_START_RX_IMMEDIATE);
    }
    // dwt_rxreset();

    // switch (event)
    // {
    // case UWB_EVENT_TIMEOUT:
    //     LOG_DBG("UWB_EVENT_TIMEOUT");
    //     break;
    // case UWB_EVENT_PACKET_RECEIVED:
    //     LOG_DBG("UWB_EVENT_PACKET_RECEIVED");
    //     break;
    // case UWB_EVENT_PACKET_SENT:
    //     LOG_DBG("UWB_EVENT_PACKET_SENT");
    //     break;
    // case UWB_EVENT_RECEIVE_TIMEOUT:
    //     LOG_DBG("UWB_EVENT_RECEIVE_TIMEOUT");
    //     break;
    // case UWB_EVENT_RECEIVE_FAILED:
    //     LOG_DBG("UWB_EVENT_RECEIVE_FAILED");
    //     break;
    // default:
    //     break;
    // }

    return UWB_TIMEOUT_MAXIMUM;
}

uwb_algorithm_t uwb_tag_algorithm = {
    .init = tag_init,
    .on_event = tag_on_event};