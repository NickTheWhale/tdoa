/**
 * @file hashmap.h
 * @author Nicholas Loehrke (nicholasnloehrke@gmail.com)
 * @brief
 * @version 1.0.0
 * @date 2024-04-11
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

#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define HASH_KEY_SIZE 8
#define HASH_CAPACITY 64

typedef struct
{
    int n;
} anchor_context_t;


typedef anchor_context_t hash_value_t;

void hash_init();
int hash_insert(uint8_t key[HASH_KEY_SIZE], hash_value_t *value);
int hash_get(uint8_t key[HASH_KEY_SIZE], hash_value_t *value);
int hash_remove(uint8_t key[HASH_KEY_SIZE]);
bool hash_empty();
size_t hash_available();

// testing code
void hash_printk();

#endif // __HASHMAP_H__
