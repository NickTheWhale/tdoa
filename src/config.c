#include "config.h"

#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/storage/flash_map.h>

LOG_MODULE_REGISTER(config, LOG_LEVEL_DBG);

#define NVS_PARTITION storage_partition
#define NVS_PARTITION_DEVICE FIXED_PARTITION_DEVICE(NVS_PARTITION)
#define NVS_PARTITION_OFFSET FIXED_PARTITION_OFFSET(NVS_PARTITION)

#define MODE_ID 0

static struct nvs_fs fs;

int config_init()
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

int config_read(struct config_t *config)
{
    ssize_t sz = nvs_read(&fs, MODE_ID, &(config->mode), sizeof(config->mode));
    if (sz != sizeof(config->mode))
    {
        return -1;
    }

    return 0;
}

int config_save(struct config_t *config)
{
    ssize_t sz = nvs_write(&fs, MODE_ID, &(config->mode), sizeof(config->mode));
    if (sz != sizeof(config->mode) || sz != 0)
    {
        return -1;
    }

    return 0;
}

int config_clear()
{
    if (nvs_delete(&fs, MODE_ID) != 0)
    {
        return -1;
    }

    return 0;
}