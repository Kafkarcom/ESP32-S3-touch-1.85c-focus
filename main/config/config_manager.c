#include "config_manager.h"
#include <cJSON.h>
#include <esp_log.h>
#include <esp_spiffs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static const char *TAG = "config_manager";
static cJSON *json_config = NULL;

static char wifi_ssid[33] = {0};
static char wifi_password[65] = {0};

esp_err_t config_init(void) {
  ESP_LOGI(TAG, "Initializing SPIFFS...");

  esp_vfs_spiffs_conf_t conf = {.base_path = "/spiffs",
                                .partition_label = "storage",
                                .max_files = 5,
                                .format_if_mount_failed = true};

  esp_err_t ret = esp_vfs_spiffs_register(&conf);

  if (ret != ESP_OK) {
    if (ret == ESP_FAIL) {
      ESP_LOGE(TAG, "Failed to mount or format filesystem");
    } else if (ret == ESP_ERR_NOT_FOUND) {
      ESP_LOGE(TAG, "Failed to find SPIFFS partition");
    } else {
      ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
    }
    return ret;
  }

  size_t total = 0, used = 0;
  ret = esp_spiffs_info(conf.partition_label, &total, &used);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)",
             esp_err_to_name(ret));
  } else {
    ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
  }

  FILE *f = fopen("/spiffs/config.json", "r");
  if (f == NULL) {
    ESP_LOGE(TAG, "Failed to open config.json");
    return ESP_FAIL;
  }

  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *string = malloc(fsize + 1);
  fread(string, 1, fsize, f);
  fclose(f);
  string[fsize] = 0;

  json_config = cJSON_Parse(string);
  free(string);

  if (json_config == NULL) {
    ESP_LOGE(TAG, "Failed to parse config.json");
    return ESP_FAIL;
  }

  cJSON *wifi = cJSON_GetObjectItemCaseSensitive(json_config, "wifi");
  if (wifi) {
    cJSON *ssid = cJSON_GetObjectItemCaseSensitive(wifi, "ssid");
    cJSON *password = cJSON_GetObjectItemCaseSensitive(wifi, "password");

    if (cJSON_IsString(ssid) && (ssid->valuestring != NULL)) {
      strncpy(wifi_ssid, ssid->valuestring, sizeof(wifi_ssid) - 1);
      ESP_LOGI(TAG, "Config SSID: %s", wifi_ssid);
    }
    if (cJSON_IsString(password) && (password->valuestring != NULL)) {
      strncpy(wifi_password, password->valuestring, sizeof(wifi_password) - 1);
      ESP_LOGI(TAG, "Config Password: %s", wifi_password);
    }
  }

  return ESP_OK;
}

const char *config_get_wifi_ssid(void) { return wifi_ssid; }

const char *config_get_wifi_password(void) { return wifi_password; }
