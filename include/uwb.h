#ifndef UWB_H
#define UWB_H

#include "deca_device_api.h"

#include <stdint.h>
#include <zephyr/kernel.h>

#define UWB_SUCCESS (0)

typedef struct 
{
  void (*init)(void);
  void (*on_event)(void);
} uwb_algorithm_t;

enum algorithms_t
{
    TAG_ALGORITHM = 0,
    ANCHOR_ALGORITHM,
    DUMMY_ALGORITHM
};

void uwb_loop(void*, void*, void*);
int uwb_init();
int uwb_listen();
int uwb_transmit(uint8* data, uint16_t length);
void uwb_change_algorithm(enum algorithms_t algo);

extern struct k_sem uwb_isr_sem;

#endif //UWB_H