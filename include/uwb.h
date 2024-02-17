#ifndef UWB_H
#define UWB_H

#include "config.h"

#include <stddef.h>
#include <stdint.h>

typedef enum uwb_mode_t
{
    TAG = 0,
    ANCHOR,
    NUMBER_OF_MODES
};

typedef struct
{
    void (*init)(void);
    void (*on_event)(void);
} uwb_algorithm_t;

int uwb_init(struct config_t *config);
void uwb_start();
int uwb_switch_mode(enum uwb_mode_t mode);
int uwb_mode_count();
char *uwb_mode_name(enum uwb_mode_t mode);

#endif // UWB_H