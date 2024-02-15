#ifndef UWB_H
#define UWB_H

#include "deca_device_api.h"

#include <stdint.h>

#define UWB_SUCCESS (0)

int uwb_init();
int uwb_listen();
int uwb_transmit(uint8* data, uint16_t length);
void test_isr(void);

extern volatile int rx_ok_callback_count;
extern volatile int rx_timeout_callback_count;
extern volatile int rx_error_callback_count;
extern volatile int tx_done_callback_count;

#endif //UWB_H