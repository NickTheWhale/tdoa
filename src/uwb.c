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

#define UWB_STACK_SIZE 2048
#define UWB_PRIORITY 0

K_THREAD_STACK_DEFINE(uwb_stack_area, UWB_STACK_SIZE);

#define TX_ANTENNA_DELAY 16436
#define RX_ANTENNA_DELAY 16436

extern uwb_algorithm_t uwb_tag_algorithm;
extern uwb_algorithm_t uwb_anchor_algorithm;
extern uwb_algorithm_t uwb_dummy_algorithm;

struct
{
    uwb_algorithm_t *algorithm;
    char *name;
} uwb_available_algorithms[] = {
    {.algorithm = &uwb_tag_algorithm, .name = "tag"},
    {.algorithm = &uwb_anchor_algorithm, .name = "anchor"},
    {.algorithm = &uwb_dummy_algorithm, .name = "dummy"},
    {NULL, NULL}};

static uwb_algorithm_t *algorithm = &uwb_dummy_algorithm;
static uint32_t timeout_ms = 0;

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

static uwb_config_t uwb_config;

static void uwb_isr(void);
static void rx_ok_callback(const dwt_cb_data_t *cb_data);
static void rx_timeout_callback(const dwt_cb_data_t *cb_data);
static void rx_error_callback(const dwt_cb_data_t *cb_data);
static void tx_done_callback(const dwt_cb_data_t *cb_data);
static void uwb_loop(void *, void *, void *);

int uwb_init()
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

    dwt_setleds(DWT_LEDS_ENABLE | DWT_LEDS_INIT_BLINK);

    if (config_read_u8(CONFIG_FIELD_MODE, &uwb_config.mode) != 0)
    {
        LOG_WRN("Failed to read UWB mode from configuration, defaulting to '%s'", uwb_mode_name(UWB_MODE_DUMMY));
        uwb_config.mode = UWB_MODE_DUMMY;
    }
    if (uwb_config.mode < uwb_mode_count())
    {
        algorithm = uwb_available_algorithms[uwb_config.mode].algorithm;
    }
    if (config_read_u8_array(CONFIG_FIELD_ADDRESS, 8, uwb_config.address) != 0)
    {
        LOG_WRN("Failed to read UWB address from configuration, defaulting to '0xffffffff'");
    }
    if (uwb_config.mode == UWB_MODE_ANCHOR)
    {
        if (config_read_u32(CONFIG_FIELD_ANCHOR_X_POS_MM, &uwb_config.anchor_x_pos_mm) != 0)
        {
            LOG_WRN("Failed to read anchor x position, defaulting to '0'");
            uwb_config.anchor_x_pos_mm = 0;
        }
        if (config_read_u32(CONFIG_FIELD_ANCHOR_Y_POS_MM, &uwb_config.anchor_y_pos_mm) != 0)
        {
            LOG_WRN("Failed to read anchor y position, defaulting to '0'");
            uwb_config.anchor_y_pos_mm = 0;
        }
    }

    return 0;
}

void uwb_start()
{
    algorithm->init(&uwb_config);

    struct k_thread uwb_thread;
    k_tid_t uwb_tid = k_thread_create(&uwb_thread, uwb_stack_area,
                                      K_THREAD_STACK_SIZEOF(uwb_stack_area),
                                      uwb_loop,
                                      NULL, NULL, NULL,
                                      UWB_PRIORITY, 0, K_NO_WAIT);
    k_thread_name_set(uwb_tid, "uwb");
}

int uwb_mode_count()
{
    return UWB_MODE_MAX;
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
        if (k_sem_take(&uwb_irq_sem, K_MSEC(timeout_ms)) == 0)
        {
            do
            {
                dwt_isr();
            } while (dwt_checkirq() != 0);
        }
        else
        {
            timeout_ms = algorithm->on_event(UWB_EVENT_TIMEOUT);
        }
    }
}

static void uwb_isr(void)
{
    k_sem_give(&uwb_irq_sem);
}

static void rx_ok_callback(const dwt_cb_data_t *cb_data)
{
    algorithm->on_event(UWB_EVENT_PACKET_RECEIVED);
}

static void rx_timeout_callback(const dwt_cb_data_t *cb_data)
{
    algorithm->on_event(UWB_EVENT_RECEIVE_TIMEOUT);
}

static void rx_error_callback(const dwt_cb_data_t *cb_data)
{
    algorithm->on_event(UWB_EVENT_RECEIVE_FAILED);
}

static void tx_done_callback(const dwt_cb_data_t *cb_data)
{
    algorithm->on_event(UWB_EVENT_PACKET_SENT);
}
