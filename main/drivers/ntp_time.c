#include "ntp_time.h"
#include "esp_log.h"
#include "esp_sntp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <time.h>
#include <sys/time.h>

static const char *TAG = "NTP";

static void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TAG, "Notification of a time synchronization event");
}

esp_err_t ntp_sync_time(uint32_t timeout_ms)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "pool.ntp.org");
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    esp_sntp_init();

    // Wait for time to be set
    time_t now = time(NULL);
    struct tm timeinfo = *localtime(&now);
    int retry = 0;
    const int retry_count = timeout_ms / 100;

    while (timeinfo.tm_year < (2016 - 1900) && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(pdMS_TO_TICKS(100));
        time(&now);
        timeinfo = *localtime(&now);
    }

    if (timeinfo.tm_year < (2016 - 1900)) {
        ESP_LOGE(TAG, "System time not set after timeout");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Time synchronized successfully");
    ntp_log_time();
    return ESP_OK;
}

void ntp_log_time(void)
{
    time_t now = time(NULL);
    struct tm timeinfo = *localtime(&now);
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    ESP_LOGI(TAG, "Current time: %s", buffer);
}
