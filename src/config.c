#include "config.h"

#include <stdbool.h>
#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/crc.h>h

LOG_MODULE_REGISTER(config, LOG_LEVEL_DBG);

#define NVS_PARTITION storage_partition
#define NVS_PARTITION_DEVICE FIXED_PARTITION_DEVICE(NVS_PARTITION)
#define NVS_PARTITION_OFFSET FIXED_PARTITION_OFFSET(NVS_PARTITION)

#define CONFIG_ID 0
#define MAGIC 0xDECA
#define VERSION_MAJOR 1
#define VERSION_MINOR 0

#define SIZE_READ 100
#define SIZE_HEADER 10

#define OFFSET_MAGIC 0
#define OFFSET_MAJOR_VERSION 2
#define OFFSET_MINOR_VERSION 3
#define OFFSET_DATA_LENGTH 4
#define OFFSET_CHECKSUM 6

typedef struct __packed config_header_t
{
    uint16_t magic;
    uint8_t major_version;
    uint8_t minor_version;
    uint16_t data_length;
    uint32_t checksum;
};

static uint8_t buffer[SIZE_READ];
static struct config_header_t *header = (struct config_header_t *)buffer;

static struct nvs_fs fs;

static int flash_init();
static bool read_data();
static bool check_data();
static bool check_magic();
static bool write_defaults();
static bool check_crc();

int config_init()
{
    if (flash_init() != 0)
    {
        LOG_ERR("Failed to initialize flash");
        return -1;
    }

    if (read_data() != 0)
    {
        LOG_ERR("Failed to read flash data");
        return -2;
    }

    if (check_data() == 0)
    {
        LOG_INF("Configuration read and verified");
    }
    else
    {
        if (write_defaults() != 0)
        {
            LOG_ERR("Failed to write default config to flash");
            return -3;
        }
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

    return 0;
}

static bool read_data()
{
    if (nvs_read(&fs, CONFIG_ID, &buffer, SIZE_READ) == 0)
    {
        LOG_ERR("Failed to read flash data");
        return false;
    }

    return true;
}

static bool check_data()
{
    if (!check_magic() || header->data_length >= (SIZE_READ - SIZE_HEADER))
    {
        LOG_ERR("Magic not found");
        return false;
    }

    return check_crc();
}

static bool check_magic()
{
    return header->magic == MAGIC;
}

static bool write_defaults()
{
    buffer[OFFSET_MAGIC] = MAGIC;
    buffer[OFFSET_MAJOR_VERSION] = VERSION_MAJOR;
    buffer[OFFSET_MINOR_VERSION] = VERSION_MINOR;
    buffer[OFFSET_CHECKSUM] = 0;
}


// static bool write_defaults(void) {
//   uint8_t default_anchor_list[] = {1, 2, 3, 4, 5, 6};

//   buffer[0] = MAGIC;
//   buffer[1] = 1; // Major version
//   buffer[2] = 0; // Minor version
//   buffer[3] = 0; // Length of TLV
//   buffer[4] = 0; // Length of TLV
//   buffer[5] = buffer[0] + buffer[1];
//   // Write the default address
//   cfgWriteU8(cfgAddress, 0);
//   cfgWriteU8(cfgMode, MODE_ANCHOR);
//   cfgWriteU8list(cfgAnchorlist, default_anchor_list, sizeof(default_anchor_list));
//   write_crc();
//   if (!eepromWrite(0, buffer, 7))
//     return false;
//   HAL_Delay(10);
//   if (readData()) {
//     if (check_content()) {
//       return true;
//     } else {
//       return false;
//     }
//   } else {
//     return false;
//   }
// }

bool check_crc()
{
    const int total_length = SIZE_HEADER + header->data_length;
    uint32_t buffer_checksum;
    memcpy(&buffer_checksum, &buffer[OFFSET_CHECKSUM], sizeof(uint32_t));
    uint32_t checksum = crc32_ieee(&buffer, total_length);

    if (buffer_checksum != checksum)
    {
        LOG_ERR("Checksums do not match");
        return false;
    }

    return true;
}