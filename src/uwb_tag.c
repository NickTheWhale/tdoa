#include "uwb.h"

#include "deca_device_api.h"
#include "deca_regs.h"
#include "deca_spi.h"
#include "mac.h"
#include "port.h"

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(tag, LOG_LEVEL_DBG);

static void tag_init(uwb_config_t *config);
static uint32_t tag_on_event(uwb_event_t event);

static void tag_init(uwb_config_t *config)
{
}

static uint32_t tag_on_event(uwb_event_t event)
{
    // if (event == UWB_EVENT_PACKET_RECEIVED)
    // {
    //     size_t packet_size = sizeof(packet_t);
    //     packet_t rx_packet;
    //     uint8 buffer[1024];
    //     uint32_t rx_length = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFLEN_MASK;
    //     dwt_readrxdata(buffer, rx_length , 0);
    // }

    // dwt_rxreset();
    // dwt_rxenable(DWT_START_RX_IMMEDIATE);

    return UWB_TIMEOUT_MAXIMUM;
}

uwb_algorithm_t uwb_tag_algorithm = {
    .init = tag_init,
    .on_event = tag_on_event};