
#include "gossip_peer_record.h"

#include "esp_timer.h"

static gossip_peer_record_t *peer_table = NULL;

gossip_peer_record_t *gossip_find_peer(const struct sockaddr_in *addr) {
    gossip_peer_record_t *rec = NULL;
    HASH_FIND(hh, peer_table, addr, sizeof(*addr), rec);
    return rec;
}

gossip_peer_record_t *gossip_add_peer(const struct sockaddr_in *addr) {
    gossip_peer_record_t *rec = calloc(1, sizeof(*rec));
    if (!rec) return NULL;

    memcpy(&rec->sockaddr, addr, sizeof(*addr));
    rec->status = GOSSIP_PEER_ALIVE;
    
    HASH_ADD(hh, peer_table, sockaddr, sizeof(*addr), rec);
    return rec;
}

int gossip_delete_peer(const struct sockaddr_in *addr) {
    gossip_peer_record_t *rec = find_peer(addr);
    if (!rec) return -1;

    HASH_DEL(peer_table, rec);
    free(rec);
    return 0;
}

void gossip_clear_all_peers(void) {
    gossip_peer_record_t *rec, *tmp;
    HASH_ITER(hh, peer_table, rec, tmp) {
        HASH_DEL(peer_table, rec);
        free(rec);
    }
}

int gossip_write_peer_record_to_buffer(void** buffer, gossip_peer_record_t* record) {
    size_t size = 1 + sizeof(struct sockaddr_in);
    *buffer = malloc(size);
    if (*buffer == NULL) return -1;

    memcpy(*buffer, &record->status, 1);
    memcpy(*buffer + 1, &record->sockaddr, sizeof(struct sockaddr_in));
    return size;
}

int gossip_read_peer_record_from_buffer(void* buffer, gossip_peer_record_t* record) {
    record->status = ((uint8_t*)buffer)[0];
    memcpy(&record->sockaddr, buffer + 1, sizeof(struct sockaddr_in));
    record->last_updated = esp_timer_get_time();
    
    // if this is a new record then we haven't really communicated with it before, set this field to current time as a placeholder.
    record->last_communicated = esp_timer_get_time();
    return 0;
}