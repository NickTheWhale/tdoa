#ifndef __PACKET_H__
#define __PACKET_H__

#include <stdint.h>

// Packet format with compressed PAN and 64Bit addresses
// Maximum 128s bytes payload
typedef struct __packed
{
    union {
        uint16_t raw;
        struct
        {
            uint16_t frame_type : 3;
            uint16_t security_enabled : 1;
            uint16_t frame_pending : 1;
            uint16_t ack_required : 1;
            uint16_t pan_id : 1;
            uint16_t reserved : 3;
            uint16_t dest_addr_mode : 2;
            uint16_t frame_version : 2;
            uint16_t src_addr_mode : 2;
        } fields;
    } frame_control;

    uint8_t sequence_number;
    uint16_t dest_pan_id;
    uint8_t dest_address[8];
    uint8_t src_address[8];

    uint8_t payload[128];
    uint8_t deca_checksum[2];
} packet_t;

#define MAC80215_PACKET_INIT(packet, TYPE) packet.frame_control.fields.frame_type = (TYPE); \
  packet.frame_control.fields.security_enabled = 0; \
  packet.frame_control.fields.frame_pending = 0; \
  packet.frame_control.fields.ack_required = 0; \
  packet.frame_control.fields.pan_id = 1; \
  packet.frame_control.fields.dest_addr_mode = 3; \
  packet.frame_control.fields.frame_version = 1; \
  packet.frame_control.fields.src_addr_mode = 3;

#define MAC802154_TYPE_BEACON 0 
#define MAC802154_TYPE_DATA 1
#define MAC802154_TYPE_ACK 2
#define MAC802154_TYPE_CMD 3

#define MAC802154_HEADER_LENGTH 21

#endif // PACKET_H
