#ifndef UWB_H
#define UWB_H

#include "deca_device_api.h"

#define UWB_SUCCESS (0)

int uwb_init();
int uwb_listen();
void test_isr(void);

#endif //UWB_H 