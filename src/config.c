#include "config.h"

#include <stdint.h>
#include <stdbool.h>
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
#define READ_LENGTH 100

typedef struct __packed config_header
{
    uint16_t magic;
    uint8_t major_version;
    uint8_t minor_version;
    uint16_t data_length;
};

static struct nvs_fs fs;
static uint8_t buffer[READ_LENGTH];

static int flash_init();
static bool read_data();
static bool check_data();
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
}

static bool read_data()
{
    if (nvs_read(&fs, CONFIG_ID, &buffer, READ_LENGTH) == 0)
    {
        return false;
    }

    return true;
}

static bool check_data()
{
    if (check_magic()/*chick size*/ )
    {
        return check_crc();
    }




    if (check_magic() && cfgHeader->tlvSize < (NUMBER_OF_BYTES_READ - SIZE_HEADER -SIZE_TAIL)) {
        return check_crc();
    }
    printf("CONFIG\t: EEPROM magic not found!\r\n");
    return false;

    return 0;
}

static bool write_defaults()
{
    return 0;
}

bool check_crc()
{
    crc32_ieee(&buffer, READ_LENGTH);
    
}
