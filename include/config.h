/**
 * @file config.h
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

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stddef.h>
#include <stdint.h>

#define CONFIG_SIZE_BUFFER 128

typedef enum
{
    CONFIG_FIELD_MODE = 0,
    CONFIG_FIELD_ADDRESS,
    CONFIG_FIELD_ANCHOR_X_POS_MM,
    CONFIG_FIELD_ANCHOR_Y_POS_MM,
    CONFIG_FIELD_MAX
} config_field_t;

int config_init();
int config_erase();
int config_refresh();
void config_buffer(uint8_t *buffer, size_t size);

int config_field_size(config_field_t field, uint8_t *size);

int config_read_u8(config_field_t field, uint8_t *value);
int config_read_u16(config_field_t field, uint16_t *value);
int config_read_u32(config_field_t field, uint32_t *value);
int config_read_u64(config_field_t field, uint64_t *value);
int config_read_float(config_field_t field, float *value);
int config_read_double(config_field_t field, double *value);

int config_write_u8(config_field_t field, uint8_t value);
int config_write_u16(config_field_t field, uint16_t value);
int config_write_u32(config_field_t field, uint32_t value);
int config_write_u64(config_field_t field, uint64_t value);
int config_write_float(config_field_t field, float value);
int config_write_double(config_field_t field, double value);

int config_read_u8_array(config_field_t field, uint8_t length, uint8_t *array);
int config_read_u16_array(config_field_t field, uint8_t length, uint16_t *array);
int config_read_u32_array(config_field_t field, uint8_t length, uint32_t *array);
int config_read_u64_array(config_field_t field, uint8_t length, uint64_t *array);
int config_read_float_array(config_field_t field, uint8_t length, float *array);
int config_read_double_array(config_field_t field, uint8_t length, double *array);

int config_write_u8_array(config_field_t field, uint8_t length, uint8_t *array);
int config_write_u16_array(config_field_t field, uint8_t length, uint16_t *array);
int config_write_u32_array(config_field_t field, uint8_t length, uint32_t *array);
int config_write_u64_array(config_field_t field, uint8_t length, uint64_t *array);
int config_write_float_array(config_field_t field, uint8_t length, float *array);
int config_write_double_array(config_field_t field, uint8_t length, double *array);

#endif // __CONFIG_H__
