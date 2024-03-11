#include "cli.h"
#include "config.h"
#include "uwb.h"

#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>

LOG_MODULE_REGISTER(cli, LOG_LEVEL_DBG);

typedef enum
{
    CLI_CONFIG_DUMP,
    CLI_CONFIG_CLEAR,
    CLI_CONFIG_REFRESH,
    CLI_CONFIG_SET_MODE_TAG,
    CLI_CONFIG_SET_MODE_ANCHOR,
    CLI_CONFIG_PRINT_MODE
} cli_command_t;

// config command
static int handle_config(const struct shell *sh,
                         size_t argc,
                         char **argv,
                         void *data);

SHELL_SUBCMD_DICT_SET_CREATE(sub_config,
                             handle_config,
                             (dump, CLI_CONFIG_DUMP, "Dump configuration buffer"),
                             (clear, CLI_CONFIG_CLEAR, "Clear configuration"),
                             (refresh, CLI_CONFIG_REFRESH, "Refresh (reread) configuration from flash"),
                             (set_tag_mode, CLI_CONFIG_SET_MODE_TAG, "Set tag mode"),
                             (set_anchor_mode, CLI_CONFIG_SET_MODE_ANCHOR, "Set anchor mode"),
                             (mode, CLI_CONFIG_PRINT_MODE, "Print mode"));

SHELL_CMD_REGISTER(config, &sub_config, "Configuration settings", NULL);
// end config command

int cli_init()
{
    return 0;
}

static int handle_config(const struct shell *sh,
                         size_t argc,
                         char **argv,
                         void *data)
{
    cli_command_t cmd = (cli_command_t)data;

    switch (cmd)
    {
    case CLI_CONFIG_DUMP:
        uint8_t buffer[CONFIG_SIZE_BUFFER];
        config_buffer(buffer, CONFIG_SIZE_BUFFER);
        LOG_HEXDUMP_INF(buffer, CONFIG_SIZE_BUFFER, "");
        break;
    case CLI_CONFIG_CLEAR:
        config_clear();
        break;
    case CLI_CONFIG_REFRESH:
        config_refresh();
        break;
    case CLI_CONFIG_SET_MODE_TAG:
        LOG_DBG("set tag");
        config_write_u8(CONFIG_FIELD_MODE, UWB_MODE_TAG);
        break;
    case CLI_CONFIG_SET_MODE_ANCHOR:
        LOG_DBG("set anchor");
        config_write_u8(CONFIG_FIELD_MODE, UWB_MODE_ANCHOR);
        break;
    case CLI_CONFIG_PRINT_MODE:
        uint8_t mode;
        if (config_read_u8(CONFIG_FIELD_MODE, &mode) != 0)
        {
            LOG_INF("Unknown mode");
        }
        else
        {
            if (mode < uwb_mode_count())
            {
                LOG_INF("Mode: %d", mode);
                LOG_INF("%s", uwb_mode_name(mode));
            }
            else
            {
                LOG_INF("Unknown mode");
            }
        }
        break;
    }

    return 0;
}