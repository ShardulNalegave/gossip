
#ifndef GOSSIP_PEER_RECORD_H
#define GOSSIP_PEER_RECORD_H

#include "stdint.h"
#include "lwip/sockets.h"
#include "vendor/uthash.h"

typedef enum {
    GOSSIP_PEER_ALIVE = 0,
    GOSSIP_PEER_FAILING = 1,
    GOSSIP_PEER_DEAD = 2,
} gossip_peer_status_t;

typedef struct gossip_peer_record_t {
    uint8_t status; // gossip_peer_status_t value
    struct sockaddr_in sockaddr;
    int64_t last_communicated;
    int64_t last_updated;

    UT_hash_handle hh;
} gossip_peer_record_t;

int gossip_write_peer_record_to_buffer(void **buffer, gossip_peer_record_t *record);
int gossip_read_peer_record_from_buffer(void *buffer, gossip_peer_record_t *record);

gossip_peer_record_t *gossip_find_peer(const struct sockaddr_in *addr);
gossip_peer_record_t *gossip_add_peer(const struct sockaddr_in *addr);
int gossip_delete_peer(const struct sockaddr_in *addr);
void gossip_clear_all_peers(void);

#endif
