#include "uwb.h"

#include "deca_device_api.h"
#include "deca_regs.h"
#include "deca_spi.h"
#include "port.h"

#include <zephyr/kernel.h>
#include <zephyr/kernel/thread.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/sem.h>

LOG_MODULE_REGISTER(uwb, LOG_LEVEL_DBG);

#define UWB_STACK_SIZE 500
#define UWB_PRIORITY 5

K_THREAD_STACK_DEFINE(uwb_stack_area, UWB_STACK_SIZE);

#define TX_ANTENNA_DELAY 16436
#define RX_ANTENNA_DELAY 16436

extern uwb_algorithm_t uwb_tag_algorithm;
extern uwb_algorithm_t uwb_anchor_algorithm;

struct
{
    uwb_algorithm_t *algorithm;
    char *name;
} uwb_available_algorithms[] = {
    {.algorithm = &uwb_tag_algorithm, .name = "Tag"},
    {.algorithm = &uwb_anchor_algorithm, .name = "Anchor"},
    {NULL, NULL}
};

static void dummy_init();
static void dummy_on_event();

static uwb_algorithm_t dummy_algorithm = {
    .init = dummy_init,
    .on_event = dummy_on_event};

static uwb_algorithm_t *algorithm = &dummy_algorithm;

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

K_SEM_DEFINE(uwb_irq_sem, 0, 1);

static void uwb_isr(void);
static void rx_ok_callback(const dwt_cb_data_t *cb_data);
static void rx_timeout_callback(const dwt_cb_data_t *cb_data);
static void rx_error_callback(const dwt_cb_data_t *cb_data);
static void tx_done_callback(const dwt_cb_data_t *cb_data);
static void uwb_loop(void *, void *, void *);

int uwb_init(config_t *config)
{
    if (openspi() != DWT_SUCCESS)
    {
        LOG_ERR("Failed to open spi");
        return -1;
    }
    port_set_dw1000_slowrate();
    if (dwt_initialise(DWT_LOADUCODE) != DWT_SUCCESS)
    {
        LOG_ERR("Failed to initialize deca");
        return -2;
    }
    port_set_dw1000_fastrate();

    dwt_configure(&dwt_config);

    dwt_settxantennadelay(TX_ANTENNA_DELAY);
    dwt_setrxantennadelay(RX_ANTENNA_DELAY);

    port_set_deca_isr(uwb_isr);

    dwt_setcallbacks(&tx_done_callback,
                     &rx_ok_callback,
                     &rx_timeout_callback,
                     &rx_error_callback);

    dwt_setinterrupt(DWT_INT_TFRS | DWT_INT_RFCG | DWT_INT_RFTO | DWT_INT_RXPTO | DWT_INT_RPHE | DWT_INT_RFCE | DWT_INT_RFSL | DWT_INT_SFDT, 1);

    dwt_setleds(DWT_LEDS_ENABLE);

    return 0;
}

void uwb_start()
{
    algorithm->init();

    struct k_thread uwb_thread;
    k_thread_create(&uwb_thread, uwb_stack_area,
                    K_THREAD_STACK_SIZEOF(uwb_stack_area),
                    uwb_loop,
                    NULL, NULL, NULL,
                    UWB_PRIORITY, 0, K_NO_WAIT);
}

int uwb_switch_mode(uwb_mode_t mode)
{
    if (mode < uwb_mode_count())
    {
        algorithm = uwb_available_algorithms[mode].algorithm;
        return 0;
    }

    return -1;
}

int uwb_mode_count()
{
    return NUMBER_OF_MODES;
}

char *uwb_mode_name(uwb_mode_t mode)
{
    if (mode >= uwb_mode_count())
    {
        return "UNKNOWN";
    }

    return uwb_available_algorithms[mode].name;
}

static void uwb_loop(void *, void *, void *)
{
    while (1)
    {
        if (k_sem_take(&uwb_irq_sem, K_MSEC(50)) == 0)
        {
            dwt_isr();
        }
        k_msleep(1);
        // LOG_DBG("uwb loop");
    }
}

static void uwb_isr(void)
{
    k_sem_give(&uwb_irq_sem);
}

static void rx_ok_callback(const dwt_cb_data_t *cb_data)
{
    algorithm->on_event();
}

static void rx_timeout_callback(const dwt_cb_data_t *cb_data)
{
    algorithm->on_event();
}

static void rx_error_callback(const dwt_cb_data_t *cb_data)
{
    algorithm->on_event();
}

static void tx_done_callback(const dwt_cb_data_t *cb_data)
{
    algorithm->on_event();
}

void dummy_init()
{
    LOG_DBG("Dummy init");
}

void dummy_on_event()
{
    LOG_DBG("Dummy on event");
}
