#include "uwb.h"

#include "deca_device_api.h"
#include "deca_regs.h"
#include "deca_spi.h"
#include "mac.h"
#include "port.h"

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(anchor, LOG_LEVEL_DBG);

static uwb_config_t *uwb_config;

static void anchor_init(uwb_config_t *config);
static uint32_t anchor_on_event(uwb_event_t event);

static void anchor_init(uwb_config_t *config)
{
    uwb_config = config;
    LOG_DBG("Anchor init");
}

static uint32_t anchor_on_event(uwb_event_t event)
{
    if (event == UWB_EVENT_TIMEOUT)
    {
        packet_t tx_packet;
        MAC80215_PACKET_INIT(&tx_packet, MAC802154_TYPE_DATA);
        memcpy(tx_packet.src_address, uwb_config->address, 8);

        dwt_writetxdata(sizeof(tx_packet), (uint8_t *)&tx_packet, 0);
        dwt_writetxfctrl(sizeof(tx_packet), 0, 1);

        dwt_starttx(DWT_START_TX_IMMEDIATE);

        uint8_t *src = tx_packet.src_address;
        uint8_t *dest = tx_packet.dest_address;

        LOG_DBG("Sending from %u:%u:%u:%u:%u:%u:%u:%u to %u:%u:%u:%u:%u:%u:%u:%u\n",
                   src[0], src[1], src[2], src[3], src[4], src[5], src[6], src[7],
                   dest[0], dest[1], dest[2], dest[3], dest[4], dest[5], dest[6], dest[7]);
    }

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

    return 2500;
}

uwb_algorithm_t uwb_anchor_algorithm = {
    .init = anchor_init,
    .on_event = anchor_on_event};