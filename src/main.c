
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
        bool need_to_listen = false;
        if (k_sem_take(&test_isr_sem, K_MSEC(50)) == 0)
        {
            printk("got test isr\n");
        }
        if (k_sem_take(&rx_ok_sem, K_MSEC(50)) == 0)
        {
            printk("got rx ok\n");
            uwb_listen();
        }
        if (k_sem_take(&rx_timeout_sem, K_MSEC(50)) == 0)
        {
            printk("got rx timeout\n");
            uwb_listen();
        }
        if (k_sem_take(&rx_error_sem, K_MSEC(50)) == 0)
        {
            printk("got rx error\n");
            uwb_listen();
        }
        if (k_sem_take(&tx_done_sem, K_MSEC(50)) == 0)
        {
            printk("got tx done\n");
        }
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

        k_msleep(3000);
    }

    return 0;
}