#include "uwb.h"

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(tag, LOG_LEVEL_DBG);

static void tag_init(void)
{
    LOG_DBG("Tag init");
}

static uint32_t tag_on_event(uwb_event_t event)   
{
    LOG_DBG("Tag on event");
    return 0;
}

uwb_algorithm_t uwb_tag_algorithm = {
    .init = tag_init,
    .on_event = tag_on_event
};