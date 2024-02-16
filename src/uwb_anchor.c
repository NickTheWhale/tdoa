#include "uwb.h"

static void anchor_init(void)
{
    printk("anchor init\n");
}

static void anchor_on_event(void)   
{
    printk("anchor on event\n");
}

uwb_algorithm_t uwb_anchor_algorithm = {
    .init = anchor_init,
    .on_event = anchor_on_event
};