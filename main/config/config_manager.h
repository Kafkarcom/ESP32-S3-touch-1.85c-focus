#pragma once

#include <esp_err.h>

/**
 * @brief Initialize the configuration system (mount SPIFFS, parse config).
 * 
 * @return esp_err_t ESP_OK on success.
 */
esp_err_t config_init(void);

/**
 * @brief Get the configured WiFi SSID.
 * 
 * @return const char* Pointer to SSID string (do not free).
 */
const char* config_get_wifi_ssid(void);

/**
 * @brief Get the configured WiFi Password.
 * 
 * @return const char* Pointer to Password string (do not free).
 */
const char* config_get_wifi_password(void);
