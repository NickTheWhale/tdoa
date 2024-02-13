#include <zephyr/sys/printk.h>

#include "uwb.h"

int main(void)
{
    int ret = uwb_init();
    if (ret != 0)
    {
        printk("Failed to initialize uwb: %d\n", ret);
    }
    else
    {
        printk("Initialized uwb\n");
    }
    
    return 0;
}