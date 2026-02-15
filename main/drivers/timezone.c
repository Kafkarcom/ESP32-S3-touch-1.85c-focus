#include "timezone.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"

static const char *TAG = "Timezone";
static const char *NVS_NAMESPACE = "storage";
static const char *NVS_KEY_TIMEZONE = "tz_offset_h";
static int s_timezone_offset_hours = 1; // Default: +1 hour for Netherlands

esp_err_t timezone_init(void) {
    // Initialize NVS if not already initialized
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGW(TAG, "NVS needs to be erased, erasing now...");
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize NVS: %s", esp_err_to_name(err));
        return err;
    }

    // Open NVS handle
    nvs_handle_t nvs_handle;
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open NVS namespace: %s", esp_err_to_name(err));
        return err;
    }

    // Try to load stored timezone offset
    int32_t stored_offset = 1; // Default fallback
    err = nvs_get_i32(nvs_handle, NVS_KEY_TIMEZONE, &stored_offset);
    
    if (err == ESP_OK) {
        s_timezone_offset_hours = (int)stored_offset;
        ESP_LOGI(TAG, "Loaded timezone offset from NVS: %d hours", s_timezone_offset_hours);
    } else if (err == ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGI(TAG, "No stored timezone offset found, using default: %d hours", s_timezone_offset_hours);
        // Store the default value
        nvs_set_i32(nvs_handle, NVS_KEY_TIMEZONE, (int32_t)s_timezone_offset_hours);
        nvs_commit(nvs_handle);
    } else {
        ESP_LOGE(TAG, "Failed to read timezone from NVS: %s", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return err;
    }

    nvs_close(nvs_handle);
    return ESP_OK;
}

int timezone_get_offset_hours(void) {
    return s_timezone_offset_hours;
}

esp_err_t timezone_set_offset_hours(int offset_hours) {
    s_timezone_offset_hours = offset_hours;

    // Persist to NVS
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open NVS: %s", esp_err_to_name(err));
        return err;
    }

    err = nvs_set_i32(nvs_handle, NVS_KEY_TIMEZONE, (int32_t)offset_hours);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set timezone in NVS: %s", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return err;
    }

    err = nvs_commit(nvs_handle);
    nvs_close(nvs_handle);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Timezone offset updated to %d hours and persisted", offset_hours);
    } else {
        ESP_LOGE(TAG, "Failed to commit timezone to NVS: %s", esp_err_to_name(err));
    }

    return err;
}

int timezone_get_offset_seconds(void) {
    return s_timezone_offset_hours * 3600;
}
