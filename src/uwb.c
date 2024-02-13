#include "uwb.h"

#include "deca_device_api.h"
#include "deca_regs.h"
#include "deca_spi.h"
#include "port.h"

int uwb_init()
{
    if (openspi() != DWT_SUCCESS)
    {
        return -1;
    }
    port_set_dw1000_slowrate();
    if (dwt_initialise(DWT_LOADUCODE) != DWT_SUCCESS)
    {
        return -2;
    }
    port_set_dw1000_fastrate();

    return 0;
}

void uwb_cleanup()
{

}