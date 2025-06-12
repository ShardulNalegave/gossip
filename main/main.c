
#include "freertos/FreeRTOS.h"
#include "nvs_flash.h"

#include "gossip_wifi.h"
#include "gossip_udp_server.h"

static char* TAG = "gossip";

void app_main(void) {
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    gossip_wifi_init();

    xTaskCreate(gossip_udp_server_start, "udp_server", 4 * 1024, NULL, 5, NULL);

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
