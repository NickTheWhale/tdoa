#ifndef UWB_H
#define UWB_H

#include "config.h"

#include <stddef.h>
#include <stdint.h>

typedef struct
{
    uint8_t mode;
    uint8_t address[8];
} uwb_config_t;

typedef enum
{
    UWB_MODE_TAG = 0,
    UWB_MODE_ANCHOR,
    UWB_MODE_MAX
} uwb_mode_t;

typedef enum
{
    UWB_EVENT_PACKET_SENT = 0,
    UWB_EVENT_PACKET_RECEIVED,
    UWB_EVENT_RECEIVE_TIMEOUT,
    UWB_EVENT_RECEIVE_FAILED
} uwb_event_t;

typedef struct
{
    void (*init)(void);
    uint32_t (*on_event)(uwb_event_t event);
} uwb_algorithm_t;

int uwb_init(uwb_config_t *config);
void uwb_start();
int uwb_mode_count();
char *uwb_mode_name(uwb_mode_t mode);

#endif // UWB_H