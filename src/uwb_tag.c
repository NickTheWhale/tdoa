#include "uwb.h"

static void tag_init(void)
{
    printk("tag init\n");
}

static void tag_on_event(void)   
{
    printk("tag on event\n");
}

uwb_algorithm_t uwb_tag_algorithm = {
    .init = tag_init,
    .on_event = tag_on_event
};