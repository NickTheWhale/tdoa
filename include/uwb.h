#ifndef __UWB_H__
#define __UWB_H__

#include "config.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#define UWB_TIMEOUT_MAXIMUM 0xffffffffUL;
#define UWB_PAN_ID 0xbeef

typedef struct
{
    uint8_t mode;
    uint8_t address[8];
    uint32_t anchor_x_pos_mm;
    uint32_t anchor_y_pos_mm;
} uwb_config_t;

typedef enum
{
    UWB_MODE_TAG = 0,
    UWB_MODE_ANCHOR,
    UWB_MODE_DUMMY,
    UWB_MODE_MAX
} uwb_mode_t;

typedef enum
{
    UWB_EVENT_TIMEOUT = 0,
    UWB_EVENT_PACKET_RECEIVED,
    UWB_EVENT_PACKET_SENT,
    UWB_EVENT_RECEIVE_TIMEOUT,
    UWB_EVENT_RECEIVE_FAILED
} uwb_event_t;

typedef enum
{
    UWB_PACKET_TYPE_SYNC = 0,
    a,
    b,
    c,
    d,
    e,
    f,
    UWB_PACKET_TYPE_MAX
} uwb_packet_type_t;

_Static_assert(UWB_PACKET_TYPE_MAX < 8, "Too many uwb packet types");

typedef struct
{
    void (*init)(uwb_config_t *config);
    uint32_t (*on_event)(uwb_event_t event);
} uwb_algorithm_t;

int uwb_init();
void uwb_start();
int uwb_mode_count();
char *uwb_mode_name(uwb_mode_t mode);

#endif // UWB_H