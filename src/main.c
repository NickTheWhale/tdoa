/**
 * @file main.c
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
#include "deca_device_api.h"
#include "hashmap.h"
#include "uwb.h"

#include <zephyr/logging/log.h>
#include <zephyr/random/random.h>
#include <zephyr/shell/shell.h>
#include <zephyr/sys/crc.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

int main(void)
{
    k_msleep(1000);

    int ret;
    ret = config_init();
    if (ret != 0)
    {
        LOG_ERR("Failed to initialize config: %d", ret);
        return -1;
    }

    ret = uwb_init();
    if (ret != 0)
    {
        LOG_ERR("Failed to initalize uwb: %d", ret);
        return -2;
    }

    LOG_DBG("Initialized uwb");

    hash_init();

    hash_value_t value;
    uint8_t key[8];
    uint8_t keys[HASH_CAPACITY][8];

    LOG_DBG("FILLING");

    for (int i = 0; i < HASH_CAPACITY; ++i)
    {
        uint64_t random_number = ((uint64_t)sys_rand32_get() << 32) | sys_rand32_get();
        value.n = random_number & 0xffffffff;
        for (int j = 0; j < 8; ++j)
        {
            key[j] = (random_number >> (8 * j)) & 0xff;
        }
        memcpy(keys[i], key, 8);

        if (hash_insert(key, &value) != 0)
        {
            LOG_ERR("Failed to insert");
            k_msleep(50);
        }

        hash_value_t read_value;
        if (hash_get(key, &read_value) != 0)
        {

            LOG_ERR("Failed to get");
            k_msleep(50);
        }

        if (read_value.n != value.n)
        {
            LOG_ERR("Value mismatch; wrote: %d, read: %d", value.n, read_value.n);
            k_msleep(50);
        }
    }

    k_msleep(1000);

    LOG_DBG("REMOVING");

    for (int i = 0; i < HASH_CAPACITY; ++i)
    {
        if (hash_remove(keys[i]) != 0)
        {
            LOG_ERR("Failed to remove");
            k_msleep(50);
        }
    }

    hash_printk();
        
    // uwb_start();

    return 0;
}