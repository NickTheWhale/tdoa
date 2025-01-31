/**
 * @file config.c
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

#include <stdbool.h>
#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/crc.h>

LOG_MODULE_REGISTER(config, LOG_LEVEL_DBG);

#define NVS_PARTITION storage_partition
#define NVS_PARTITION_DEVICE FIXED_PARTITION_DEVICE(NVS_PARTITION)
#define NVS_PARTITION_OFFSET FIXED_PARTITION_OFFSET(NVS_PARTITION)

#define CONFIG_NVS_ID 0
#define MAGIC 0xBEEF
#define VERSION_MAJOR 1
#define VERSION_MINOR 0

#define SIZE_READ (CONFIG_SIZE_BUFFER)
#define SIZE_HEADER 6
#define SIZE_TAIL 4
#define SIZE_TLV_MAX (SIZE_READ - SIZE_HEADER - SIZE_TAIL)

#define OFFSET_MAGIC 0
#define OFFSET_MAJOR_VERSION 2
#define OFFSET_MINOR_VERSION 3

#define CONFIG_READ_DELAY 300

typedef struct __packed
{
    uint16_t magic;
    uint8_t major_version;
    uint8_t minor_version;
    uint16_t tlv_length;
} config_header_t;

typedef struct
{
    uint8_t *data;
} tlv_t;

static uint8_t _buffer[SIZE_READ];
static config_header_t *header = (config_header_t *)_buffer;
static tlv_t tlv;

static struct nvs_fs fs;

static int flash_init();
static int read_buffer();
static int check_data();
static int check_magic();
static int write_defaults();
static int check_crc();
static void write_crc();
static int find_field(config_field_t field);
static int read_value(config_field_t field, uint8_t size, void *value);
static int write_value(config_field_t field, uint8_t size, void *value);
static int insert_value(int field_pos, config_field_t field, uint8_t size, void *value);
static int append_value(config_field_t field, uint8_t size, void *value);

int config_init()
{
    int ret;

    ret = flash_init();
    if (ret != 0)
    {
        LOG_ERR("Failed to initialize flash: %d", ret);
        return -1;
    }

    k_msleep(CONFIG_READ_DELAY);
    ret = read_buffer();
    if (ret != 0)
    {
        LOG_ERR("Failed to read flash data: %d", ret);
        return -2;
    }

    tlv.data = &_buffer[SIZE_HEADER];

    ret = check_data();
    if (ret == 0)
    {
        LOG_INF("Configuration read and verified");
    }
    else
    {
        ret = write_defaults();
        if (ret != 0)
        {
            LOG_ERR("Failed to write default config to flash: %d", ret);
            return -3;
        }
        LOG_INF("Wrote default config to flash");
    }

    return 0;
}

int config_erase()
{
    if (nvs_delete(&fs, CONFIG_NVS_ID) != 0)
    {
        return -1;
    }

    return 0;
}

static int flash_init()
{
    struct flash_pages_info info;
    fs.flash_device = NVS_PARTITION_DEVICE;
    if (!device_is_ready(fs.flash_device))
    {
        LOG_ERR("Flash device is not ready");
        return -1;
    }
    fs.offset = NVS_PARTITION_OFFSET;
    if (flash_get_page_info_by_offs(fs.flash_device, fs.offset, &info) != 0)
    {
        LOG_ERR("Unabled to get page info");
        return -2;
    }
    fs.sector_size = info.size;
    fs.sector_count = 3U;
    if (nvs_mount(&fs) != 0)
    {
        LOG_ERR("Failed to mount flash");
        return -3;
    }

    int dummy_buffer[1];
    k_msleep(CONFIG_READ_DELAY);
    if (nvs_read(&fs, CONFIG_NVS_ID, &dummy_buffer, sizeof(dummy_buffer)) < 0)
    {
        if (nvs_write(&fs, CONFIG_NVS_ID, &dummy_buffer, sizeof(dummy_buffer)) < 0)
        {
            LOG_ERR("Failed to write null config");
            return -4;
        }
    }

    return 0;
}

static int read_buffer()
{
    int ret;
    ret = nvs_read(&fs, CONFIG_NVS_ID, &_buffer, SIZE_READ);
    if (ret < 0)
    {
        return ret;
    }

    return 0;
}

static int check_data()
{
    if (check_magic() != 0)
    {
        LOG_ERR("Magic not found");
        return -1;
    }

    if (header->tlv_length >= (SIZE_READ - SIZE_HEADER))
    {
        LOG_ERR("Data length mismatch");
        return -2;
    }

    return check_crc();
}

static int check_magic()
{
    const uint16_t magic = (MAGIC);
    if (memcmp(&header->magic, &magic, sizeof(uint16_t)))
    {
        LOG_ERR("header magic: %u, magic: %u", header->magic, magic);
        return -1;
    }

    return 0;
}

static int write_defaults()
{
    const uint16_t magic = (MAGIC);
    memcpy(&_buffer[OFFSET_MAGIC], &magic, sizeof(uint16_t));

    _buffer[OFFSET_MAJOR_VERSION] = VERSION_MAJOR;
    _buffer[OFFSET_MINOR_VERSION] = VERSION_MINOR;

    write_crc();

    if (nvs_write(&fs, CONFIG_NVS_ID, &_buffer, SIZE_READ) < 0)
    {
        return -1;
    }

    k_msleep(CONFIG_READ_DELAY);
    if (read_buffer() != 0)
    {
        return -2;
    }

    if (check_data() != 0)
    {
        return -3;
    }

    return 0;
}

static int check_crc()
{
    const int total_length = SIZE_HEADER + header->tlv_length;
    uint32_t buffer_checksum;
    memcpy(&buffer_checksum, &_buffer[SIZE_HEADER + header->tlv_length], sizeof(uint32_t));
    uint32_t checksum = crc32_ieee(_buffer, total_length);

    if (buffer_checksum != checksum)
    {
        LOG_ERR("Checksums do not match. Reference: %u, Calculated: %u", buffer_checksum, checksum);
        return -1;
    }

    return 0;
}

static void write_crc()
{
    uint32_t checksum = crc32_ieee(_buffer, SIZE_HEADER + header->tlv_length);
    memcpy(&_buffer[SIZE_HEADER + header->tlv_length], &checksum, sizeof(uint32_t));
}

static int find_field(config_field_t field)
{
    int pos = 0;
    while (pos < header->tlv_length)
    {
        if (tlv.data[pos] == field)
        {
            return pos;
        }

        pos += tlv.data[pos + 1] + 2;
    }

    return -1;
}

int config_refresh()
{
    k_msleep(CONFIG_READ_DELAY);
    if (read_buffer() != 0)
    {
        LOG_ERR("Failed to refresh config");
        return -1;
    }

    return 0;
}

void config_buffer(uint8_t *buffer, size_t size)
{
    size_t read_size = MIN(size, SIZE_READ);
    memcpy(buffer, &_buffer, read_size);
}

int config_field_size(config_field_t field, uint8_t *size)
{
    int pos = find_field(field);
    if (pos < 0)
    {
        return -1;
    }

    *size = tlv.data[pos + 1];

    return 0;
}

int read_value(config_field_t field, uint8_t size, void *value)
{
    int pos = find_field(field);
    if (pos < 0)
    {
        return -1;
    }
    if (pos + 2 + size >= (SIZE_TLV_MAX))
    {
        LOG_ERR("Field size exceeds maximum TLV length");
        return -2;
    }
    if (size != tlv.data[pos + 1])
    {
        LOG_ERR("Field size mismatch when reading");
        return -3;
    }
    memcpy(value, &tlv.data[pos + 2], size);

    return 0;
}

int write_value(config_field_t field, uint8_t size, void *value)
{
    const int bytes_to_insert = 2 + size;
    const int bytes_avaliable = SIZE_TLV_MAX - header->tlv_length;
    if (bytes_to_insert > bytes_avaliable)
    {
        LOG_ERR("Cannot add value to flash, not enough space");
        return -1;
    }

    int bytes_inserted = 0;
    int pos = find_field(field);
    if (pos > -1)
    {
        bytes_inserted = insert_value(pos, field, size, value);
    }
    else
    {
        bytes_inserted = append_value(field, size, value);
    }

    if (bytes_inserted < 0)
    {
        LOG_ERR("Failed to insert value into buffer");
        return -2;
    }

    write_crc();
    ssize_t bytes_written = nvs_write(&fs, CONFIG_NVS_ID, &_buffer, SIZE_READ);
    if (bytes_written < 0)
    {
        LOG_ERR("Failed to write value to flash");
        return -3;
    }

    k_msleep(CONFIG_READ_DELAY);
    if (read_buffer() != 0)
    {
        LOG_ERR("Failed to re-read buffer after write");
        return -4;
    }

    return 0;
}

static int insert_value(int field_pos, config_field_t field, uint8_t size, void *value)
{
    const int bytes_to_insert = 2 + size;
    const int bytes_avaliable = SIZE_TLV_MAX - header->tlv_length;
    if (bytes_to_insert > bytes_avaliable)
    {
        LOG_ERR("Field size exceeds maximum TLV length");
        return -1;
    }

    uint8_t old_size = tlv.data[field_pos + 1];
    if (memcmp(&tlv.data[field_pos + 2], value, size) != 0 || size != old_size)
    {
        if (size != old_size) // shift data
        {
            const int src_idx = field_pos + 2 + old_size;
            const int dest_idx = field_pos + 2 + size;
            const size_t move_size = header->tlv_length - (field_pos + 2 + old_size);
            memmove(&tlv.data[dest_idx], &tlv.data[src_idx], move_size);
        }

        tlv.data[field_pos] = field;
        tlv.data[field_pos + 1] = size;
        memcpy(&tlv.data[field_pos + 2], value, size);
        header->tlv_length += size - old_size;
    }

    return size;
}

static int append_value(config_field_t field, uint8_t size, void *value)
{
    const int bytes_to_insert = 2 + size;
    const int bytes_avaliable = SIZE_TLV_MAX - header->tlv_length;
    if (bytes_to_insert > bytes_avaliable)
    {
        LOG_ERR("Field size exceeds maximum TLV length");
        return -1;
    }

    tlv.data[header->tlv_length] = field;
    tlv.data[header->tlv_length + 1] = size;
    memcpy(&tlv.data[header->tlv_length + 2], value, size);
    header->tlv_length += bytes_to_insert;

    return size;
}

// TODO check bounds when passing in uint8_t length
int config_read_u8(config_field_t field, uint8_t *value)
{
    return read_value(field, sizeof(uint8_t), value);
}

int config_read_u16(config_field_t field, uint16_t *value)
{
    return read_value(field, sizeof(uint16_t), value);
}

int config_read_u32(config_field_t field, uint32_t *value)
{
    return read_value(field, sizeof(uint32_t), value);
}

int config_read_u64(config_field_t field, uint64_t *value)
{
    return read_value(field, sizeof(uint64_t), value);
}

int config_read_float(config_field_t field, float *value)
{
    return read_value(field, sizeof(float), value);
}

int config_read_double(config_field_t field, double *value)
{
    return read_value(field, sizeof(double), value);
}

int config_write_u8(config_field_t field, uint8_t value)
{
    return write_value(field, sizeof(uint8_t), &value);
}

int config_write_u16(config_field_t field, uint16_t value)
{
    return write_value(field, sizeof(uint16_t), &value);
}

int config_write_u32(config_field_t field, uint32_t value)
{
    return write_value(field, sizeof(uint32_t), &value);
}

int config_write_u64(config_field_t field, uint64_t value)
{
    return write_value(field, sizeof(uint64_t), &value);
}

int config_write_float(config_field_t field, float value)
{
    return write_value(field, sizeof(float), &value);
}

int config_write_double(config_field_t field, double value)
{
    return write_value(field, sizeof(double), &value);
}

int config_read_u8_array(config_field_t field, uint8_t length, uint8_t *array)
{
    return read_value(field, sizeof(uint8_t) * length, array);
}

int config_read_u16_array(config_field_t field, uint8_t length, uint16_t *array)
{
    return read_value(field, sizeof(uint16_t) * length, array);
}

int config_read_u32_array(config_field_t field, uint8_t length, uint32_t *array)
{
    return read_value(field, sizeof(uint32_t) * length, array);
}

int config_read_u64_array(config_field_t field, uint8_t length, uint64_t *array)
{
    return read_value(field, sizeof(uint64_t) * length, array);
}

int config_read_float_array(config_field_t field, uint8_t length, float *array)
{
    return read_value(field, sizeof(float) * length, array);
}

int config_read_double_array(config_field_t field, uint8_t length, double *array)
{
    return read_value(field, sizeof(double) * length, array);
}

int config_write_u8_array(config_field_t field, uint8_t length, uint8_t *array)
{
    return write_value(field, sizeof(uint8_t) * length, array);
}

int config_write_u16_array(config_field_t field, uint8_t length, uint16_t *array)
{
    return write_value(field, sizeof(uint16_t) * length, array);
}

int config_write_u32_array(config_field_t field, uint8_t length, uint32_t *array)
{
    return write_value(field, sizeof(uint32_t) * length, array);
}

int config_write_u64_array(config_field_t field, uint8_t length, uint64_t *array)
{
    return write_value(field, sizeof(uint64_t) * length, array);
}

int config_write_float_array(config_field_t field, uint8_t length, float *array)
{
    return write_value(field, sizeof(float) * length, array);
}

int config_write_double_array(config_field_t field, uint8_t length, double *array)
{
    return write_value(field, sizeof(double) * length, array);
}
