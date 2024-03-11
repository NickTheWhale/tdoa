#include "uwb.h"

#include "deca_device_api.h"
#include "deca_regs.h"
#include "deca_spi.h"
#include "mac.h"
#include "port.h"

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(anchor, LOG_LEVEL_DBG);

static void anchor_init(uwb_config_t *config);
static uint32_t anchor_on_event(uwb_event_t event);

static void anchor_init(uwb_config_t *config)
{
}

static uint32_t anchor_on_event(uwb_event_t event)
{
    // if (event == UWB_EVENT_TIMEOUT)
    // {
    //     packet_t tx_packet;
    //     MAC80215_PACKET_INIT(tx_packet, MAC802154_TYPE_DATA);

    //     dwt_writetxdata(sizeof(tx_packet), (uint8_t *)&tx_packet, 0);
    //     dwt_writetxfctrl(sizeof(tx_packet), 0, 1);

    //     dwt_starttx(DWT_START_TX_IMMEDIATE);
    // }

    return 1000;
}

uwb_algorithm_t uwb_anchor_algorithm = {
    .init = anchor_init,
    .on_event = anchor_on_event};