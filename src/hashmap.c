/**
 * @file hashmap.c
 * @author Nicholas Loehrke (nicholasnloehrke@gmail.com)
 * @brief
 * @version 1.0.0
 * @date 2024-04-12
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

#include "hashmap.h"

#include <zephyr/logging/log.h>
#include <zephyr/sys/crc.h>

LOG_MODULE_REGISTER(hashmap, LOG_LEVEL_DBG);

static size_t VALUE_SIZE;
typedef struct
{
    void *value;
    bool occupied;
} hash_entry_t;

static hash_entry_t buckets[HASH_MAX_BUCKETS];

static uint32_t hash(uint8_t key[HASH_KEY_SIZE]);

void hash_init(size_t value_size)
{
    VALUE_SIZE = value_size;
}

int hash_insert(uint8_t key[HASH_KEY_SIZE], void *value)
{
    int index = hash(key) % HASH_MAX_BUCKETS;
    if (buckets[index].occupied)
    {
        LOG_ERR("HASHMAP COLLISION");
        return -1;
    }

    buckets[index].value = value;
    buckets[index].occupied = true;

    return 0;
}

int hash_get(uint8_t key[HASH_KEY_SIZE], void *value)
{
    int index = hash(key) % HASH_MAX_BUCKETS;
    if (!buckets[index].occupied)
    {
        return -1;
    }

    value = buckets[index].value;
    return 0;
}

int hash_remove(uint8_t key[HASH_KEY_SIZE])
{
    return 0;
}

static uint32_t hash(uint8_t key[HASH_KEY_SIZE])
{
    return crc32_ieee(key, HASH_KEY_SIZE);
}