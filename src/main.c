
#include "uwb.h"
#include "deca_device_api.h"

#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>


extern void tag_thread(void*, void*, void*);
extern void anchor_thread(void*, void*, void*);

int main(void)
{

    int ret = uwb_init();
    if (ret != UWB_SUCCESS)
    {
        printk("Failed to initialize uwb: %d\n", ret);
        return -1;
    }

    printk("Initialized uwb\n");

    #if(APP_MODE==TAG_MODE)
    return tag_loop();
    #elif(APP_MODE==ANCHOR_MODE)
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
        uwb_listen();
        k_msleep(1000);
        printk("rx ok: %d\nrx timeout: %d\nrx error: %d\ntx done: %d",
                rx_ok_callback_count,
                rx_timeout_callback_count,
                rx_error_callback_count,
                tx_done_callback_count);
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
        else
        {
            printk("Transmitted\n");
        }

        k_msleep(2000);
    }

    return 0;
}