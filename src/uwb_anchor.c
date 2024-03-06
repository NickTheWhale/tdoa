#include "uwb.h"

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(anchor, LOG_LEVEL_DBG);

static void anchor_init(void)
{
    LOG_DBG("Anchor init");
}

static uint32_t anchor_on_event(uwb_event_t event)   
{
    LOG_DBG("Anchor on event");
    return 0;
}

uwb_algorithm_t uwb_anchor_algorithm = {
    .init = anchor_init,
    .on_event = anchor_on_event
};