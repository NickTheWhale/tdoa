#include "uwb.h"

#include "deca_device_api.h"
#include "deca_regs.h"
#include "deca_spi.h"
#include "port.h"

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(tag, LOG_LEVEL_DBG);


static uint32_t tag_on_event(uwb_event_t event)
{

    switch (event)
    {
    case UWB_EVENT_PACKET_RECEIVED:
        uint32_t frame_len = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFLEN_MASK;
        if (frame_len <= RX_BUF_LEN)
        {
            dwt_readrxdata(rx_buffer, frame_len, 0);
        }
    case UWB_EVENT_PACKET_RECEIVED:
    case UWB_EVENT_PACKET_SENT:
    case UWB_EVENT_RECEIVE_TIMEOUT:
    case UWB_EVENT_RECEIVE_FAILED:
        break;
    }

    return 0;
}

uwb_algorithm_t uwb_tag_algorithm = {
    .init = tag_init,
    .on_event = tag_on_event};