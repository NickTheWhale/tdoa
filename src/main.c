#include "config.h"
#include "deca_device_api.h"
#include "uwb.h"

#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

static int mode_cmd_handler(const struct shell *sh,
                                 size_t argc,
                                 char **argv,
                                 void *data);

SHELL_SUBCMD_DICT_SET_CREATE(sub_mode,
                                mode_cmd_handler,
                                (tag, TAG, "Switch to 'tag' mode"),
                                (anchor, ANCHOR, "Switch to 'anchor' mode"));

SHELL_CMD_REGISTER(mode, &sub_mode, "Change UWB algorithm settings", NULL);

static struct config_t config;

int main(void)
{
    k_msleep(500);
    if (config_init() != 0)
    {
        LOG_ERR("Failed to initialze config");
        return -1;
    }
    LOG_DBG("Initialized config");

    if (config_read(&config) != 0)
    {
        LOG_ERR("Failed to read config");
        return -2;
    }

    if (uwb_init(&config) != 0)
    {
        LOG_ERR("Failed to initalize uwb");
        return -3;
    }
    LOG_DBG("Initialized uwb");

    uwb_start();

    return 0;
}

int algorithm_cmd_handler(const struct shell *sh, size_t argc, char **argv, void *data)
{
    
    return 0;
}
