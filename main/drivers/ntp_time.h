#ifndef NTP_TIME_H
#define NTP_TIME_H

#include "esp_err.h"
#include <time.h>

/**
 * @brief Synchronize time with NTP server
 * @param timeout_ms Maximum time to wait for NTP sync in milliseconds
 * @return ESP_OK on success, error code on failure
 */
esp_err_t ntp_sync_time(uint32_t timeout_ms);

/**
 * @brief Get current time and log it
 */
void ntp_log_time(void);

#endif // NTP_TIME_H
