
#include "uwb.h"
#include "deca_device_api.h"

#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>

int main(void)
{
    int ret = uwb_init();
    if (ret != UWB_SUCCESS)
    {
        printk("Failed to initialize uwb: %d\n", ret);
        return -1;
    }

    printk("Initialized uwb\n");

    ret = uwb_listen();
    if (ret != UWB_SUCCESS)
    {
        printk("Failed to start uwb listen\n");
        return -2;
    }

    printk("Listening...\n");

    while (1)
    {
        dwt_isr();
        // k_msleep(10);
    }
    
    return 0;
}