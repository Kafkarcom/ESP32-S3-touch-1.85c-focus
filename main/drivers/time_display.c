#include "time_display.h"
#include "esp_log.h"
#include "lvgl.h"
#include <time.h>
#include <string.h>

static const char *TAG = "TimeDisplay";
static lv_obj_t *s_time_label = NULL;
static lv_timer_t *s_time_timer = NULL;

// Timer callback - runs on LVGL task (thread-safe)
static void time_update_timer_cb(lv_timer_t *timer)
{
    if (s_time_label == NULL) {
        return;
    }

    time_t now = time(NULL);
    struct tm timeinfo = *localtime(&now);
    
    // Format: HH:MM:SS for clock-like display
    char time_str[16];
    strftime(time_str, sizeof(time_str), "%H:%M:%S", &timeinfo);
    
    lv_label_set_text(s_time_label, time_str);
}

void time_display_init(lv_obj_t *time_label)
{
    if (time_label == NULL) {
        ESP_LOGE(TAG, "Invalid time label pointer");
        return;
    }

    s_time_label = time_label;
    ESP_LOGI(TAG, "Time display initialized");

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
    s_time_label = NULL;
    ESP_LOGI(TAG, "Time display stopped");
}