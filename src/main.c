#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "deca_device_api.h"
#include "deca_regs.h"
#include "deca_spi.h"
#include "port.h"

int init_deca()
{
        openspi();
        port_set_dw1000_slowrate();
        if (dwt_initialise(DWT_LOADUCODE) == DWT_ERROR)
        {
                return -1;
        }
        port_set_dw1000_fastrate();

        return 0;
}

int main(int argc, char** argv)
{
        int ret;
        ret = init_deca();
        if (ret != 0)
        {
                return -1;
        }
        
        printk("wassup\n");
        
        return 0;
}