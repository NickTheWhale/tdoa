#ifndef CONFIG_H
#define CONFIG_H

#include <stddef.h>
#include <stdint.h>

typedef struct
{
    uint8_t mode;
} config_t;

typedef enum
{
    CONFIG_FIELD_MODE = 0
} config_field_t;

int config_init();
int config_clear();
void config_buffer(uint8_t *buffer, size_t size);

int config_read_u8(config_field_t field, uint8_t *value);
int config_write_u8(config_field_t field, uint8_t value);

#endif // CONFIG_H
