/**
 * @file cli.c
 * @author Nicholas Loehrke (nicholasnloehrke@gmail.com)
 * @brief
 * @version 1.0.0
 * @date 2024-03-19
 *
 * MIT License
 *
 * Copyright (c) 2024 Nicholas Loehrke
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

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
    shell_warn(shell, "NOT IMPLEMENTED");

    return 0;
}

static int cmd_config_erase(const struct shell *shell, size_t argc, char **argv)
{
    if (config_erase() != 0)
    {
        shell_error(shell, "Failed to erase flash");
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
            shell_error(shell, "Failed to set UWB mode to '%s'", argv[1]);
            return -1;
        }
        else
        {
            shell_info(shell, "Set UWB mode to '%s'", argv[1]);
        }
    }
    else
    {
        uint8_t mode;
        ret = config_read_u8(CONFIG_FIELD_MODE, &mode);
        if (ret != 0)
        {
            shell_error(shell, "Failed to retrieve current UWB mode");
            return -2;
        }
        else
        {
            shell_info(shell, "Current UWB mode is '%s'", uwb_mode_name(mode));
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

        uint8_t new_address[8];
        int i = 0;
        while ((token = strtok_r(rest, ":;,.", &rest)) && i < 8)
        {
            int err = 0;
            unsigned long value = shell_strtoul(token, 16, &err);
            if (err != 0)
            {
                shell_error(shell, "Invalid address format. Failed at '%s'", token);
                return -1;
            }

            if (value > 0xff)
            {
                shell_error(shell, "Invalid address format at '0x%lx'", value);
                return -2;
            }

            new_address[i] = value;

            ++i;
        }

        if (i != 8)
        {
            shell_error(shell, "Invalid address length. Got '%u', expected '%u'", i, 8);
            return -3;
        }

        if (config_write_u8_array(CONFIG_FIELD_ADDRESS, 8, new_address) != 0)
        {
            shell_error(shell, "Failed to write address to config");
            return -4;
        }

        uint8_t address[8];
        if (config_read_u8_array(CONFIG_FIELD_ADDRESS, 8, address) != 0)
        {
            shell_error(shell, "Failed to re-read address from config");
        }

        shell_fprintf(shell, SHELL_VT100_COLOR_GREEN, "Set UWB address to ");
        i = 0;
        for (; i < 7; ++i)
        {
            shell_fprintf(shell, SHELL_VT100_COLOR_GREEN, "%x:", address[i]);
        }
        shell_fprintf(shell, SHELL_VT100_COLOR_GREEN, "%x\n", address[i]);
    }
    else
    {
        uint8_t address[8];
        if (config_read_u8_array(CONFIG_FIELD_ADDRESS, 8, address) != 0)
        {
            shell_error(shell, "Failed to retrieve current address");
            return -4;
        }

        shell_fprintf(shell, SHELL_VT100_COLOR_GREEN, "Current UWB address is ");
        int i = 0;
        for (; i < 7; ++i)
        {
            shell_fprintf(shell, SHELL_VT100_COLOR_GREEN, "%x:", address[i]);
        }
        shell_fprintf(shell, SHELL_VT100_COLOR_GREEN, "%x\n", address[i]);
    }
    return 0;
}

static int cmd_config_anchor_position(const struct shell *shell, size_t argc, char **argv)
{
    if (!(argc == 1 || argc == 3))
    {
        shell_error(shell, "Invalid usage. Expected 0 or 2 arguments");
        return -1;
    }
    if (argc > 2)
    {
        uint32_t new_x, new_y, old_x, old_y;
        int err = 0;
        new_x = shell_strtoul(argv[1], 10, &err);
        if (err != 0)
        {
            shell_error(shell, "Invalid format for 'x' coordinate");
            return -2;
        }
        new_y = shell_strtoul(argv[2], 10, &err);
        if (err != 0)
        {
            shell_error(shell, "Invalid format for 'y' coordinate");
            return -3;
        }
        if (config_write_u32(CONFIG_FIELD_ANCHOR_X_POS_MM, new_x) != 0)
        {
            shell_error(shell, "Failed to write 'x' coordinate");
            return -4;
        }
        if (config_write_u32(CONFIG_FIELD_ANCHOR_Y_POS_MM, new_y) != 0)
        {
            shell_error(shell, "Failed to write 'y' coordinate");
            return -5;
        }
        if (config_read_u32(CONFIG_FIELD_ANCHOR_X_POS_MM, &old_x) != 0)
        {
            shell_error(shell, "Failed to re-read 'x' coordinate");
            return -6;
        }
        if (config_read_u32(CONFIG_FIELD_ANCHOR_Y_POS_MM, &old_y) != 0)
        {
            shell_error(shell, "Failed to re-read 'y' coordinate");
            return -7;
        }
        if (old_x != new_x || old_y != new_y)
        {
            shell_error(shell, "Failed to write coordinates");
            return -8;
        }

        shell_info(shell, "Set coordinates to x: %u, y: %u", new_x, new_y);
        return 0;
    }
    else if (argc == 1)
    {
        uint32_t x, y;
        if (config_read_u32(CONFIG_FIELD_ANCHOR_X_POS_MM, &x) != 0 || config_read_u32(CONFIG_FIELD_ANCHOR_Y_POS_MM, &y) != 0)
        {
            shell_error(shell, "Failed to read anchor position");
            return -9;
        }
        shell_info(shell, "x: %u (mm), y: %u (mm)", x, y);
    }
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(config_sub,
                               SHELL_CMD(dump, NULL, "Dump configuration in hexadecimal format", cmd_config_dump),
                               SHELL_CMD(print, NULL, "Print configuration in human-readable format", cmd_config_print),
                               SHELL_CMD(erase, NULL, "Erase configuration from flash", cmd_config_erase),
                               SHELL_CMD_ARG(mode, NULL, "Set/Get UWB mode", cmd_config_mode, 1, 1),
                               SHELL_CMD_ARG(address, NULL, "Set/Get UWB address", cmd_config_address, 1, 1),
                               SHELL_CMD_ARG(anchor_position, NULL, "Set/Get UWB anchor position", cmd_config_anchor_position, 1, 2),
                               SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(config, &config_sub, "Configuration commands", NULL);
