#ifndef UWB_H
#define UWB_H

#include "deca_device_api.h"

#include <stdint.h>

#define UWB_SUCCESS (0)

int uwb_init();
int uwb_listen();
int uwb_transmit(uint8* data, uint16_t length);
void test_isr(void);

#endif //UWB_H 