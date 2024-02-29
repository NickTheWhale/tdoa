#include "config.h"
#include "deca_device_api.h"
#include "uwb.h"

#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>
#include <zephyr/sys/crc.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

static uwb_config_t uwb_config;

int main(void)
{
    k_msleep(1000); // wait for shell to startup

    int ret;
    ret = config_init();
    if (ret != 0)
    {
        LOG_ERR("Failed to initialize config: %d", ret);
        return -1;
    }

    if (config_read_u8(CONFIG_FIELD_MODE, &uwb_config.mode) != 0)
        LOG_WRN("Failed to read mode from config");

    if (config_read_u8_array(CONFIG_FIELD_ADDRESS, 8, uwb_config.address) != 0)
        LOG_WRN("Failed to read address from config");

    ret = uwb_init(&uwb_config);
    if (ret != 0)
    {
        LOG_ERR("Failed to initalize uwb: %d", ret);
        return -2;
    }

    LOG_DBG("Initialized uwb");

    uwb_start();

    return 0;
}