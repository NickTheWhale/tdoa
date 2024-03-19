/**
 * @file uwb_dummy.c
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

#include "uwb.h"

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(dummy, LOG_LEVEL_DBG);

static void dummy_init(uwb_config_t *config);
static uint32_t dummy_on_event(uwb_event_t event);

static void dummy_init(uwb_config_t *config)
{
    LOG_DBG("Dummy init");
}

static uint32_t dummy_on_event(uwb_event_t event)
{
    LOG_DBG("Dummy on event");
    return UWB_TIMEOUT_MAXIMUM;
}

uwb_algorithm_t uwb_dummy_algorithm = {
    .init = dummy_init,
    .on_event = dummy_on_event};
