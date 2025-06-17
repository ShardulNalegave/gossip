
#ifndef GOSSIP_MESSAGE_H
#define GOSSIP_MESSAGE_H

#include "stdint.h"

typedef enum {
    GOSSIP = 0,
    SEND_ALL_PEERS = 1
} gossip_message_kind_t;

typedef struct {
    uint8_t kind; // gossip_message_kind_t value
    uint32_t length;
} gossip_message_header_t;

#endif
