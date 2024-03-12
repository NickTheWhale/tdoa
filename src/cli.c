#include "config.h"
#include "uwb.h"

#include <stdint.h>
#include <stdio.h>
#include <zephyr/shell/shell.h>

static int cmd_config_dump(const struct shell *shell, size_t argc, char **argv)
{
    uint8_t buffer[CONFIG_SIZE_BUFFER];
    config_buffer(buffer, CONFIG_SIZE_BUFFER);
    shell_hexdump(shell, buffer, CONFIG_SIZE_BUFFER);

    return 0;
}

static int cmd_config_print(const struct shell *shell, size_t argc, char **argv)
{
    shell_print(shell, "NOT IMPLEMENTED");

    return 0;
}

static int cmd_config_erase(const struct shell *shell, size_t argc, char **argv)
{
    if (config_erase() != 0)
    {
        shell_print(shell, "Failed to erase flash");
        return -1;
    }

    return 0;
}

static int cmd_config_mode(const struct shell *shell, size_t argc, char **argv)
{
    int ret;
    if (argc > 1)
    {
        if (strcmp(argv[1], "tag") == 0 || strcmp(argv[1], "0") == 0)
        {
            argv[1] = "tag";
            ret = config_write_u8(CONFIG_FIELD_MODE, UWB_MODE_TAG);
        }
        else if (strcmp(argv[1], "anchor") == 0 || strcmp(argv[1], "1") == 0)
        {
            argv[1] = "anchor";
            ret = config_write_u8(CONFIG_FIELD_MODE, UWB_MODE_ANCHOR);
        }
        else
        {
            ret = -1;
        }

        if (ret != 0)
        {
            shell_print(shell, "Failed to set UWB mode to '%s'", argv[1]);
            return -1;
        }
        else
        {
            shell_print(shell, "Set UWB mode to '%s'", argv[1]);
        }
    }
    else
    {
        uint8_t mode;
        ret = config_read_u8(CONFIG_FIELD_MODE, &mode);
        if (ret != 0)
        {
            shell_print(shell, "Failed to retrieve current UWB mode");
            return -2;
        }
        else
        {
            shell_print(shell, "Current UWB mode is %s", uwb_mode_name(mode));
        }
    }

    return 0;
}

static int cmd_config_address(const struct shell *shell, size_t argc, char **argv)
{
    if (argc > 1)
    {
        char *str = argv[1];
        char *token;
        char *rest = str;

        while ((token = strtok_r(rest, ":;,.", &rest)))
        {
            shell_print(shell, "%s", token);
        }
    }
    else
    {
        // Print current UWB address
        shell_print(shell, "Current UWB address is ...");
    }
    return 0;
}

static int cmd_config_anchor_position(const struct shell *shell, size_t argc, char **argv)
{
    shell_print(shell, "NOT IMPLEMENTED");
    if (argc > 1)
    {
    }
    else
    {
    }
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(config_sub,
                               SHELL_CMD(dump, NULL, "Dump configuration in hexadecimal format", cmd_config_dump),
                               SHELL_CMD(print, NULL, "Print configuration in human-readable format", cmd_config_print),
                               SHELL_CMD(erase, NULL, "Erase configuration from flash", cmd_config_erase),
                               SHELL_CMD_ARG(mode, NULL, "Set/Get UWB mode", cmd_config_mode, 1, 1),
                               SHELL_CMD_ARG(address, NULL, "Set/Get UWB address", cmd_config_address, 1, 1),
                               SHELL_CMD_ARG(anchor_position, NULL, "Set/Get UWB anchor position", cmd_config_anchor_position, 1, 1),
                               SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(config, &config_sub, "Configuration commands", NULL);
