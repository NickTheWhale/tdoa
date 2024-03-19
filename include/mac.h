#ifndef __PACKET_H__
#define __PACKET_H__

#include "uwb.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>

#define MAC80215_PACKET_SIZE (sizeof(mac_packet_t))
#define MAC802154_PACKET_HEADER_SIZE 21
#define MAC80215_PACKET_PAYLOAD_SIZE 128
#define MAC80215_PACKET_TAIL_SIZE 2

// Packet format with compressed PAN and 64Bit addresses
// Maximum 128 bytes payload
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

    uint8_t payload[MAC80215_PACKET_PAYLOAD_SIZE];
    uint8_t deca_checksum[2];
} mac_packet_t;

typedef enum
{
    MAC802154_TYPE_BEACON = 0,
    MAC802154_TYPE_DATA,
    MAC802154_TYPE_ACK,
    MAC802154_TYPE_CMD
} mac_packet_type_t;

/**
 * @brief Initialize packet header
 * @param packet: pointer to mac_packet_t packet
 * @param type: mac_packet_type_t type
 */
#define MAC80215_PACKET_INIT(packet, type)                   \
    do                                                       \
    {                                                        \
        (packet)->frame_control.fields.frame_type = (type);  \
        (packet)->frame_control.fields.security_enabled = 0; \
        (packet)->frame_control.fields.frame_pending = 0;    \
        (packet)->frame_control.fields.ack_required = 0;     \
        (packet)->frame_control.fields.pan_id = UWB_PAN_ID;  \
        (packet)->frame_control.fields.dest_addr_mode = 3;   \
        (packet)->frame_control.fields.frame_version = 1;    \
        (packet)->frame_control.fields.src_addr_mode = 3;    \
    } while (0)

/**
 * @brief LOG_DBG packet contents
 * @param packet: pointer to mac_packet_t packet
 */
#define MAC80215_LOG_PACKET(packet)                                                   \
    LOG_DBG("frame_type: %u", (packet)->frame_control.fields.frame_type);             \
    LOG_DBG("security_enabled: %u", (packet)->frame_control.fields.security_enabled); \
    LOG_DBG("frame_pending: %u", (packet)->frame_control.fields.frame_pending);       \
    LOG_DBG("ack_required: %u", (packet)->frame_control.fields.ack_required);         \
    LOG_DBG("pan_id: %u", (packet)->frame_control.fields.pan_id);                     \
    LOG_DBG("reserved: %u", (packet)->frame_control.fields.reserved);                 \
    LOG_DBG("dest_addr_mode: %u", (packet)->frame_control.fields.dest_addr_mode);     \
    LOG_DBG("frame_version: %u", (packet)->frame_control.fields.frame_version);       \
    LOG_DBG("src_addr_mode: %u", (packet)->frame_control.fields.src_addr_mode);       \
    LOG_DBG("sequence_number: %u", (packet)->sequence_number);                        \
    LOG_DBG("dest_pan_id: %u", (packet)->dest_pan_id);                                \
    LOG_HEXDUMP_DBG((packet)->dest_address, 8, "dest_address");                       \
    LOG_HEXDUMP_DBG((packet)->src_address, 8, "src_address");                         \
    LOG_HEXDUMP_DBG((packet)->payload, MAC80215_PACKET_PAYLOAD_SIZE, "payload");             \
    LOG_HEXDUMP_DBG((packet)->deca_checksum, 2, "deca_checksum");

#endif // PACKET_H
