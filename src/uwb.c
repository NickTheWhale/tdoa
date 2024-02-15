#include "uwb.h"

#include "deca_device_api.h"
#include "deca_regs.h"
#include "deca_spi.h"
#include "port.h"

#include <zephyr/sys/printk.h>

#define TX_ANTENNA_DELAY 16436
#define RX_ANTENNA_DELAY 16436

volatile int rx_ok_callback_count = 0;
volatile int rx_timeout_callback_count = 0;
volatile int rx_error_callback_count = 0;
volatile int tx_done_callback_count = 0;

static dwt_config_t dwt_config = {
    5,               /* Channel number. */
    DWT_PRF_64M,     /* Pulse repetition frequency. */
    DWT_PLEN_128,    /* Preamble length. Used in TX only. */
    DWT_PAC8,        /* Preamble acquisition chunk size. Used in RX only. */
    9,               /* TX preamble code. Used in TX only. */
    9,               /* RX preamble code. Used in RX only. */
    1,               /* 0 to use standard SFD, 1 to use non-standard SFD. */
    DWT_BR_6M8,      /* Data rate. */
    DWT_PHRMODE_EXT, /* PHY header mode. */
    (129)            /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
};


static void rx_ok_callback(const dwt_cb_data_t *cb_data);
static void rx_timeout_callback(const dwt_cb_data_t *cb_data);
static void rx_error_callback(const dwt_cb_data_t *cb_data);
static void tx_done_callback(const dwt_cb_data_t *cb_data);

int uwb_init()
{
    rx_ok_callback_count = 0;
    rx_timeout_callback_count = 0;
    rx_error_callback_count = 0;
    tx_done_callback_count = 0;

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

    dwt_configure(&dwt_config);

    dwt_settxantennadelay(TX_ANTENNA_DELAY);
    dwt_setrxantennadelay(RX_ANTENNA_DELAY);

    port_set_deca_isr(test_isr);

    dwt_setcallbacks(&tx_done_callback,
                     &rx_ok_callback,
                     &rx_timeout_callback,
                     &rx_error_callback);

    dwt_setinterrupt(DWT_INT_TFRS 
                   | DWT_INT_RFCG 
                   | DWT_INT_RFTO 
                   | DWT_INT_RXPTO
                   | DWT_INT_RPHE 
                   | DWT_INT_RFCE 
                   | DWT_INT_RFSL 
                   | DWT_INT_SFDT, 1);

    dwt_setleds(DWT_LEDS_ENABLE);

    return UWB_SUCCESS;
}

int uwb_listen()
{
    dwt_setrxtimeout(0);
    if (dwt_rxenable(DWT_START_RX_IMMEDIATE) != DWT_SUCCESS)
    {
        return -1;
    }

    return UWB_SUCCESS;
}

int uwb_transmit(uint8 *data, uint16_t length)
{
    if (dwt_writetxdata(length, data, 0) != DWT_SUCCESS)
    {
        return -1;
    }
    dwt_writetxfctrl(length, 0, 0);

    if (dwt_starttx(DWT_START_TX_IMMEDIATE) != DWT_SUCCESS)
    {
        return -2;
    }

    return UWB_SUCCESS;
}

void test_isr(void)
{
    dwt_isr();
}

static void rx_ok_callback(const dwt_cb_data_t *cb_data)
{
    // printk("rx ok callback\n");
    ++rx_ok_callback_count;
}

static void rx_timeout_callback(const dwt_cb_data_t *cb_data)
{
    // printk("rx timeout callback\n");
    ++rx_timeout_callback_count;
}

static void rx_error_callback(const dwt_cb_data_t *cb_data)
{
    // printk("rx error callback\n");
    ++rx_error_callback_count;
}

static void tx_done_callback(const dwt_cb_data_t *cb_data)
{
    // printk("tx done callback\n");
    ++tx_done_callback_count;
}