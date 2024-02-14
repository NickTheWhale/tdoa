
#include "uwb.h"
#include "deca_device_api.h"

#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>

int tag_loop();
int anchor_loop();

int main(void)
{

    int ret = uwb_init();
    if (ret != UWB_SUCCESS)
    {
        printk("Failed to initialize uwb: %d\n", ret);
        return -1;
    }

    printk("Initialized uwb\n");

    #if(APP_MODE==0)
    return tag_loop();
    #else
    return anchor_loop();
    #endif
    
    return 0;
}

int tag_loop()
{   
    int ret;
    ret = uwb_listen();
    if (ret != UWB_SUCCESS)
    {
        printk("Failed to start uwb listen\n");
        return -1;
    }
    printk("Listening...\n");
    while (1)
    {
        k_msleep(10);
        dwt_isr();
    }

    return 0;
}

int anchor_loop()
{
    uint8 data[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    while (1)
    {
        if (uwb_transmit(data, 10) != UWB_SUCCESS)
        {
            printk("Failed to transmit\n");
        }

        k_msleep(1000);
        
        dwt_isr();
    }

    return 0;
}