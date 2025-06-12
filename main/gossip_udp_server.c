
#include "gossip_udp_server.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "errno.h"
#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"

#define GOSSIP_UDP_SERVER_PORT 12500

static char* TAG = "gossip_udp_server";

void gossip_udp_server_start(void *arg) {
    char buffer[1024];
    struct sockaddr_in server_addr, client_addr;
    socklen_t socklen = sizeof(client_addr);

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock < 0) {
        ESP_LOGE(TAG, "socket() failed: errno = %d\n\t%s", errno, strerror(errno));
        goto cleanup;
    }

    ESP_LOGI(TAG, "Socket created");
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port        = htons(GOSSIP_UDP_SERVER_PORT);

    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        ESP_LOGE(TAG, "bind() failed: errno %d\n\t%s", errno, strerror(errno));
        goto cleanup;
    }

    ESP_LOGI(TAG, "Listening for connections");
    while (1) {
        int len = recvfrom(
            sock,
            buffer,
            sizeof(buffer) - 1,
            0,
            (struct sockaddr *)&client_addr,
            &socklen
        );

        if (len < 0) {
            ESP_LOGE(TAG, "recvfrom() failed: errno %d\n\t%s", errno, strerror(errno));
            break;
        }

        buffer[len] = '\0';  // null‑terminate
        ESP_LOGI(
            TAG, "Received %d bytes from %s:%d",
            len,
            ipaddr_ntoa((const ip_addr_t*)&client_addr.sin_addr),
            ntohs(client_addr.sin_port)
        );
        ESP_LOGI(TAG, "\t%s", buffer);
    }

cleanup:
    ESP_LOGI(TAG, "Shutting down UDP server");
    if (sock >= 0) close(sock);
    vTaskDelete(NULL);
}
