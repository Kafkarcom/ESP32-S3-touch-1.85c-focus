#ifndef TIMEZONE_H
#define TIMEZONE_H

#include "esp_err.h"

/**
 * @brief Initialize timezone module and load stored offset from NVS
 * @return ESP_OK on success, ESP_FAIL otherwise
 */
esp_err_t timezone_init(void);

/**
 * @brief Get the current timezone offset in hours (e.g., +1 for Netherlands)
 * @return Timezone offset in hours
 */
int timezone_get_offset_hours(void);

/**
 * @brief Set and persist the timezone offset in NVS
 * @param offset_hours Timezone offset in hours
 * @return ESP_OK on success, ESP_FAIL otherwise
 */
esp_err_t timezone_set_offset_hours(int offset_hours);

/**
 * @brief Get the timezone offset in seconds (for system time adjustment)
 * @return Timezone offset in seconds
 */
int timezone_get_offset_seconds(void);

#endif // TIMEZONE_H
