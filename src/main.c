#include "config.h"
#include "deca_device_api.h"
#include "uwb.h"

#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>
#include <zephyr/sys/crc.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

static config_t config;

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

    // testing config

    for (int i = 0; i < 42; ++i)
    {
        if (config_write_u8(i, (char)i) != 0)
        {
            LOG_DBG("Failed to write: %d", i);
        }
    }


    // end testing config

    ret = uwb_init(&config);
    if (ret != 0)
    {
        LOG_ERR("Failed to initalize uwb: %d", ret);
        return -2;
    }
    LOG_DBG("Initialized uwb");

    uwb_start();

    return 0;
}