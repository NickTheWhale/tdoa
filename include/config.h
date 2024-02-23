#ifndef CONFIG_H
#define CONFIG_H

#include <stddef.h>
#include <stdint.h>

#define CONFIG_SIZE_BUFFER 128

typedef struct
{
    uint8_t mode;
} config_t;

typedef enum
{
    CONFIG_FIELD_MODE = 0,
    CONFIG_FIELD_TEST_1 = 1,
    CONFIG_FIELD_TEST_2 = 2,
    CONFIG_FIELD_TEST_3 = 3,
    CONFIG_FIELD_TEST_4 = 4,
    CONFIG_FIELD_TEST_5 = 5,
    CONFIG_FIELD_TEST_6 = 6,
    CONFIG_FIELD_TEST_7 = 7,
    CONFIG_FIELD_TEST_8 = 8,
    CONFIG_FIELD_TEST_9 = 9,
    CONFIG_FIELD_TEST_10 = 10,
    CONFIG_FIELD_TEST_11 = 11,
    CONFIG_FIELD_TEST_12 = 12,
    CONFIG_FIELD_TEST_13 = 13,
    CONFIG_FIELD_TEST_14 = 14,
    CONFIG_FIELD_TEST_15 = 15,
    CONFIG_FIELD_TEST_16 = 16,
    CONFIG_FIELD_TEST_17 = 17,
    CONFIG_FIELD_TEST_18 = 18,
    CONFIG_FIELD_TEST_19 = 19,
    CONFIG_FIELD_TEST_20 = 20,
    CONFIG_FIELD_TEST_21 = 21,
    CONFIG_FIELD_TEST_22 = 22,
    CONFIG_FIELD_TEST_23 = 23,
    CONFIG_FIELD_TEST_24 = 24,
    CONFIG_FIELD_TEST_25 = 25,
    CONFIG_FIELD_TEST_26 = 26,
    CONFIG_FIELD_TEST_27 = 27,
    CONFIG_FIELD_TEST_28 = 28,
    CONFIG_FIELD_TEST_29 = 29,
    CONFIG_FIELD_TEST_30 = 30,
    CONFIG_FIELD_TEST_31 = 31,
    CONFIG_FIELD_TEST_32 = 32,
    CONFIG_FIELD_TEST_33 = 33,
    CONFIG_FIELD_TEST_34 = 34,
    CONFIG_FIELD_TEST_35 = 35,
    CONFIG_FIELD_TEST_36 = 36,
    CONFIG_FIELD_TEST_37 = 37,
    CONFIG_FIELD_TEST_38 = 38,
    CONFIG_FIELD_TEST_39 = 39,
    CONFIG_FIELD_TEST_40 = 40,
    CONFIG_FIELD_TEST_41 = 41,
    CONFIG_FIELD_TEST_42 = 42,
    CONFIG_FIELD_TEST_43 = 43,
    CONFIG_FIELD_TEST_44 = 44
} config_field_t;

int config_init();
int config_clear();
int config_refresh();
void config_buffer(uint8_t *buffer, size_t size);

int config_field_size(config_field_t field, uint8_t *size);

int config_read_u8(config_field_t field, uint8_t *value);
int config_read_u16(config_field_t field, uint16_t *value);
int config_read_u32(config_field_t field, uint32_t *value);

int config_write_u8(config_field_t field, uint8_t value);
int config_write_u16(config_field_t field, uint16_t value);
int config_write_u32(config_field_t field, uint32_t value);

int config_read_u8_array(config_field_t field, uint8_t length, uint8_t *array);
int config_read_u16_array(config_field_t field, uint8_t length, uint16_t *array);
int config_read_u32_array(config_field_t field, uint8_t length, uint32_t *array);

int config_write_u8_array(config_field_t field, uint8_t length, uint8_t *array);
int config_write_u16_array(config_field_t field, uint8_t length, uint16_t *array);
int config_write_u32_array(config_field_t field, uint8_t length, uint32_t *array);

#endif // CONFIG_H
