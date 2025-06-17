
set(H_SOURCES
    "gossip_wifi.h"
    "gossip_udp_server.h"
    "gossip_peer_record.h"
    "gossip_message.h"
    "vendor/uthash.h")
set(C_SOURCES
    "main.c"
    "gossip_wifi.c"
    "gossip_udp_server.c"
    "gossip_peer_record.c"
    "gossip_message.c")

idf_component_register(
    SRCS ${H_SOURCES} ${C_SOURCES}
    PRIV_REQUIRES esp_wifi nvs_flash esp_timer
    INCLUDE_DIRS "."
)