#include "time_display.h"
#include "esp_log.h"
#include "lvgl.h"
#include <time.h>
#include <string.h>

static const char *TAG = "TimeDisplay";
static lv_obj_t *s_hour_label = NULL;
static lv_obj_t *s_minute_label = NULL;
static lv_obj_t *s_second_label = NULL;
static lv_timer_t *s_time_timer = NULL;

// Timer callback - runs on LVGL task (thread-safe)
static void time_update_timer_cb(lv_timer_t *timer)
{
    if (s_hour_label == NULL || s_minute_label == NULL || s_second_label == NULL) {
        return;
    }

    time_t now = time(NULL);
    struct tm timeinfo = *localtime(&now);
    
    // Format: HH, MM, and SS separately
    char hour_str[4];
    char min_str[4];
    char sec_str[4];
    strftime(hour_str, sizeof(hour_str), "%H", &timeinfo);
    strftime(min_str, sizeof(min_str), "%M", &timeinfo);
    strftime(sec_str, sizeof(sec_str), "%S", &timeinfo);
    
    lv_label_set_text(s_hour_label, hour_str);
    lv_label_set_text(s_minute_label, min_str);
    lv_label_set_text(s_second_label, sec_str);
}

void time_display_init(lv_obj_t *hour_label, lv_obj_t *minute_label, lv_obj_t *second_label)
{
    if (hour_label == NULL || minute_label == NULL || second_label == NULL) {
        ESP_LOGE(TAG, "Invalid time label pointers");
        return;
    }

    s_hour_label = hour_label;
    s_minute_label = minute_label;
    s_second_label = second_label;
    ESP_LOGI(TAG, "Time display initialized (Split: HH MM SS)");

    // Create LVGL timer to update time periodically (runs on LVGL task - thread-safe)
    s_time_timer = lv_timer_create(time_update_timer_cb, 500, NULL);
    if (s_time_timer == NULL) {
        ESP_LOGE(TAG, "Failed to create time update timer");
    }
}

void time_display_update(void)
{
    // Direct update - can be called from anywhere (thread-safe via LVGL)
    if (s_time_timer != NULL) {
        // The timer callback will be called periodically, no need to call manually
        time_update_timer_cb(s_time_timer);
    }
}

void time_display_stop(void)
{
    if (s_time_timer != NULL) {
        lv_timer_del(s_time_timer);
        s_time_timer = NULL;
    }
    s_hour_label = NULL;
    s_minute_label = NULL;
    s_second_label = NULL;
    ESP_LOGI(TAG, "Time display stopped");
}