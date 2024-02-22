#include "cli.h"
#include "config.h"
#include "uwb.h"

#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(cli, LOG_LEVEL_DBG);

typedef enum
{
    CLI_CONFIG_DUMP,
    CLI_CONFIG_CLEAR,
    CLI_CONFIG_REFRESH
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
                             (refresh, CLI_CONFIG_REFRESH, "Refresh (reread) configuration from flash"));

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
    }
    return 0;
}