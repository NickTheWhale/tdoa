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
#define MAGIC 0xDECA
#define VERSION_MAJOR 1
#define VERSION_MINOR 0

#define SIZE_READ 1024
#define SIZE_HEADER 6
#define SIZE_TAIL 4

#define OFFSET_MAGIC 0
#define OFFSET_MAJOR_VERSION 2
#define OFFSET_MINOR_VERSION 3
#define OFFSET_DATA_LENGTH 4

#define DELAY_MS 200

typedef struct __packed 
{
    uint16_t magic;
    uint8_t major_version;
    uint8_t minor_version;
    uint16_t data_length;
} config_header_t;

static uint8_t _buffer[SIZE_READ];
static config_header_t *header = (config_header_t *)_buffer;

static struct nvs_fs fs;

static int flash_init();
static int read_data();
static int check_data();
static int check_magic();
static int write_defaults();
static int check_crc();
void config_log_buffer();

int config_init()
{
    int ret;

    ret = flash_init();
    if (ret != 0)
    {
        LOG_ERR("Failed to initialize flash: %d", ret);
        return -1;
    }

    k_msleep(DELAY_MS);
    ret = read_data();
    if (ret != 0)
    {
        LOG_ERR("Failed to read flash data: %d", ret);
        return -2;
    }

    k_msleep(DELAY_MS);
    ret = check_data();
    if (ret == 0)
    {
        LOG_INF("Configuration read and verified");
    }
    else
    {
        ret = write_defaults();
        k_msleep(DELAY_MS);
        if (ret != 0)
        {
            LOG_ERR("Failed to write default config to flash: %d", ret);
            return -3;
        }
        LOG_INF("Wrote default config to flash");
    }

    return 0;
}

int config_clear()
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

static int read_data()
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
    
    if (header->data_length >= (SIZE_READ - SIZE_HEADER))
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

    const size_t total_length = SIZE_HEADER + header->data_length;
    uint32_t checksum = crc32_ieee(_buffer, total_length);

    memcpy(&_buffer[SIZE_HEADER + header->data_length], &checksum, sizeof(uint32_t));

    if (nvs_write(&fs, CONFIG_NVS_ID, &_buffer, SIZE_READ) < 0)
    {
        return -1;
    }

    if (read_data() != 0)
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
    const int total_length = SIZE_HEADER + header->data_length;
    uint32_t buffer_checksum;
    memcpy(&buffer_checksum, &_buffer[SIZE_HEADER + header->data_length], sizeof(uint32_t));
    uint32_t checksum = crc32_ieee(_buffer, total_length);

    if (buffer_checksum != checksum)
    {
        LOG_ERR("Checksums do not match. Reference: %u, Calculated: %u", buffer_checksum, checksum);
        return -1;
    }

    return 0;
}

void config_buffer(uint8_t* buffer, size_t size)
{
    size_t read_size = MIN(size, SIZE_READ);
    memcpy(buffer, &_buffer, read_size);
}