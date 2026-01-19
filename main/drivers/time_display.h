#ifndef TIME_DISPLAY_H
#define TIME_DISPLAY_H

#include "lvgl.h"

/**
 * @brief Initialize time display and start update task
 * @param time_label Pointer to the LVGL label object for time display
 */
void time_display_init(lv_obj_t *time_label);

/**
 * @brief Update the time label with current time
 */
void time_display_update(void);

/**
 * @brief Stop the time display task
 */
void time_display_stop(void);

#endif // TIME_DISPLAY_H
