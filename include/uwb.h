#ifndef UWB_H
#define UWB_H

#include "deca_device_api.h"

#include <stdint.h>
#include <zephyr/kernel.h>

#define UWB_SUCCESS (0)

int uwb_init();
int uwb_listen();
int uwb_transmit(uint8* data, uint16_t length);
void test_isr(void);

extern struct k_sem test_isr_sem;
extern struct k_sem rx_ok_sem;  
extern struct k_sem rx_timeout_sem;  
extern struct k_sem rx_error_sem;  
extern struct k_sem tx_done_sem;  

#endif //UWB_H