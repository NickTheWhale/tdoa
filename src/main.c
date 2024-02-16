
#include "deca_device_api.h"
#include "uwb.h"

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define THREAD_START_DELAY 1000
#define THREAD_PRIORITY 7
#define THREAD_STACK_SIZE 500

#if (APP_MODE == TAG_MODE)
void tag_thread(void *, void *, void *);

K_THREAD_DEFINE(tag_thread_id,
                THREAD_STACK_SIZE,
                tag_thread,
                NULL, NULL, NULL,
                THREAD_PRIORITY, 0,
                THREAD_START_DELAY);

#elif (APP_MODE == ANCHOR_MODE)
void anchor_thread(void *, void *, void *);

K_THREAD_DEFINE(anchor_thread_id,
                THREAD_STACK_SIZE,
                anchor_thread,
                NULL, NULL, NULL,
                THREAD_PRIORITY, 0,
                THREAD_START_DELAY);
#endif

void dwt_isr_thread(void *, void *, void *);
K_THREAD_DEFINE(dwt_isr_thread_id,
                THREAD_STACK_SIZE,
                dwt_isr_thread,
                NULL, NULL, NULL,
                THREAD_PRIORITY, 0, 
                THREAD_START_DELAY);

int main(void)
{
    int ret = uwb_init();
    if (ret != UWB_SUCCESS)
    {
        printk("Failed to intialize uwb: %d\n", ret);
        return -1;
    }

    printk("Initialized\n");

    return 0;
}

void tag_thread(void *, void *, void *)
{
    int ret = uwb_listen();
    if (ret != UWB_SUCCESS)
    {
        printk("Failed to start uwb listen\n");
        return;
    }

    printk("Listening...\n");

    while (1)
    {
    }
}

void anchor_thread(void *, void *, void *)
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

        k_msleep(1000);
    }
}

void dwt_isr_thread(void *, void *, void *)
{
    while (1)
    {
        if (k_sem_take(&uwb_isr_sem, K_MSEC(50)) == 0)
        {
            dwt_isr();
        }
    }
}
