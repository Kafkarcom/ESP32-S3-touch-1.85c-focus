#ifndef WIFI_CONNECTION_H
#define WIFI_CONNECTION_H

#include "esp_err.h"

/**
 * @brief Initialize WiFi connection
 * @return ESP_OK on success, error code on failure
 */
esp_err_t wifi_init(void);

#endif // WIFI_CONNECTION_H
