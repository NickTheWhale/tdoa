#include "uwb_utils.h"

uint64_t uwb_utils_timestamp_to_u64(uint8_t *timestamp_buffer)
{
    uint64_t ts = 0;
    int i;
    for (i = 4; i >= 0; i--)
    {
        ts <<= 8;
        ts |= timestamp_buffer[i];
    }
    return ts;
}
