/**
 * @file uwb.h
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

#ifndef __UWB_H__
#define __UWB_H__

#include "config.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#define UWB_TIMEOUT_MAXIMUM 0xFFFFFFFFUL;
#define UWB_PAN_ID 0xBEEF

typedef struct
{
    uint8_t mode;
    uint8_t address[8];
    uint32_t anchor_x_pos_mm;
    uint32_t anchor_y_pos_mm;
} uwb_config_t;

typedef enum
{
    UWB_MODE_TAG = 0,
    UWB_MODE_ANCHOR,
    UWB_MODE_DUMMY,
    UWB_MODE_MAX
} uwb_mode_t;

typedef enum
{
    UWB_EVENT_TIMEOUT = 0,
    UWB_EVENT_PACKET_RECEIVED,
    UWB_EVENT_PACKET_SENT,
    UWB_EVENT_RECEIVE_TIMEOUT,
    UWB_EVENT_RECEIVE_FAILED
} uwb_event_t;

typedef enum
{
    UWB_PACKET_TYPE_SYNC = 0,
    UWB_PACKET_TYPE_MAX
} uwb_packet_type_t;

_Static_assert(UWB_PACKET_TYPE_MAX < 8, "Too many uwb packet types");

typedef struct
{
    void (*init)(uwb_config_t *config);
    uint32_t (*on_event)(uwb_event_t event);
} uwb_algorithm_t;

int uwb_init();
void uwb_start();
int uwb_mode_count();
char *uwb_mode_name(uwb_mode_t mode);

#endif // UWB_H