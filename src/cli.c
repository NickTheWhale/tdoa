#include "cli.h"
#include "config.h"
#include "uwb.h"

#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(cli, LOG_LEVEL_DBG);

typedef enum
{
    CLI_CONFIG_PRINT,
    CLI_CONFIG_CLEAR
} cli_command_t;

// config command
static int handle_config(const struct shell *sh,
                         size_t argc,
                         char **argv,
                         void *data);

SHELL_SUBCMD_DICT_SET_CREATE(sub_config,
                             handle_config,
                             (print, CLI_CONFIG_PRINT, "Print configuration"),
                             (clear, CLI_CONFIG_CLEAR, "Clear configuration"));

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
    case CLI_CONFIG_PRINT:
        uint8_t buffer[100];
        config_buffer(buffer, 100);
        LOG_HEXDUMP_INF(buffer, 100, "");
        break;
    case CLI_CONFIG_CLEAR:
        config_clear();
        break;
    }
    return 0;
}