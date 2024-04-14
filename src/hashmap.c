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

#include <assert.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>

LOG_MODULE_REGISTER(hashmap, LOG_LEVEL_DBG);

#define HASH_MAX_BUCKETS 128

_Static_assert(HASH_MAX_BUCKETS > HASH_CAPACITY, "Invalid HASH_CAPACITY");

typedef struct
{
    uint8_t key[HASH_KEY_SIZE];
    hash_value_t value;
    bool occupied;
} hash_bucket_t;

static hash_bucket_t buckets[HASH_MAX_BUCKETS];

static size_t occupied_count = 0;

static uint32_t hash(uint8_t key[HASH_KEY_SIZE]);

void hash_init()
{
    for (int i = 0; i < HASH_MAX_BUCKETS; ++i)
    {
        buckets[i].occupied = false;
    }

    occupied_count = 0;
}

int hash_insert(uint8_t key[HASH_KEY_SIZE], hash_value_t *value)
{
    if (occupied_count >= HASH_CAPACITY)
        return -1;

    int index = hash(key) % HASH_MAX_BUCKETS;
    for (int i = 0; i < HASH_MAX_BUCKETS; ++i)
    {
        int try = (index + i) % HASH_MAX_BUCKETS;
        if (buckets[try].occupied && memcmp(buckets[try].key, key, HASH_KEY_SIZE) == 0)
        {
            memcpy(&buckets[try].value, value, sizeof(hash_value_t));
            return 0;
        }

        if (!buckets[try].occupied)
        {
            memcpy(buckets[try].key, key, HASH_KEY_SIZE);
            memcpy(&buckets[try].value, value, sizeof(hash_value_t));
            buckets[try].occupied = true;
            ++occupied_count;
            return 0;
        }
    }

    return -2;
}

int hash_get(uint8_t key[HASH_KEY_SIZE], hash_value_t *value)
{
    int index = hash(key) % HASH_MAX_BUCKETS;
    for (int i = 0; i < HASH_MAX_BUCKETS; ++i)
    {
        int try = (index + i) % HASH_MAX_BUCKETS;
        if (buckets[try].occupied && memcmp(buckets[try].key, key, HASH_KEY_SIZE) == 0)
        {
            *value = buckets[try].value;
            return 0;
        }
        if (!buckets[try].occupied)
        {
            return -1;
        }
    }

    return -2;
}

int hash_remove(uint8_t key[HASH_KEY_SIZE])
{
    int index = hash(key) % HASH_MAX_BUCKETS;
    for (int i = 0; i < HASH_MAX_BUCKETS; ++i)
    {
        int try = (index + i) % HASH_MAX_BUCKETS;
        if (buckets[try].occupied && memcmp(buckets[try].key, key, HASH_KEY_SIZE) == 0)
        {
            buckets[try].occupied = false;
            --occupied_count;
            return 0;
        }
    }

    return -1;
}

bool hash_empty()
{
    return occupied_count == 0;
}

size_t hash_available()
{
    return HASH_CAPACITY - occupied_count;
}

static uint32_t hash(uint8_t key[HASH_KEY_SIZE])
{
#define FNV_PRIME 0x01000193
#define FNV_OFFSET_BASIS 0x811c9dc5

    uint32_t h = FNV_OFFSET_BASIS;
    for (int i = 0; i < HASH_KEY_SIZE; ++i)
    {
        h ^= key[i];
        h *= FNV_PRIME;
    }

    return h;
}

// testing code
void hash_printk()
{
    for (int i = 0; i < HASH_MAX_BUCKETS; ++i)
    {
        hash_bucket_t b = buckets[i];
        if (b.occupied)
        {
            printk("~~~~~~~~~~~~bucket %d~~~~~~~~~~~~\n", i);
            printk("key:      %u:%u:%u:%u:%u:%u:%u:%u\n", b.key[0], b.key[1], b.key[2], b.key[3], b.key[4], b.key[5], b.key[6], b.key[7]);
            printk("value:    %d\n", b.value.n);
            printk("occupied: %d\n\n", b.occupied);

            k_msleep(50);
        }
    }

    printk("done.\n\n\n");
}