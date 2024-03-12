#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stddef.h>
#include <stdint.h>

#define CONFIG_SIZE_BUFFER 128

typedef enum
{
    CONFIG_FIELD_MODE = 0,
    CONFIG_FIELD_ADDRESS
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

#endif // CONFIG_H
