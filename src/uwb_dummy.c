#include "uwb.h"

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(dummy, LOG_LEVEL_DBG);

static void dummy_init(uwb_config_t *config);
static uint32_t dummy_on_event(uwb_event_t event);

static void dummy_init(uwb_config_t *config)
{
    LOG_DBG("Dummy init");
}

static uint32_t dummy_on_event(uwb_event_t event)
{
    LOG_DBG("Dummy on event");
    return UWB_TIMEOUT_MAXIMUM;
}

uwb_algorithm_t uwb_dummy_algorithm = {
    .init = dummy_init,
    .on_event = dummy_on_event};
