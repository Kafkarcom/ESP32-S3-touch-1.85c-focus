#ifndef TIME_DISPLAY_H
#define TIME_DISPLAY_H

#include "lvgl.h"

/**
 * @brief Initialize time display and start update task
 * @param hour_label Pointer to the LVGL label object for hours
 * @param minute_label Pointer to the LVGL label object for minutes
 * @param second_label Pointer to the LVGL label object for seconds
 */
void time_display_init(lv_obj_t *hour_label, lv_obj_t *minute_label, lv_obj_t *second_label);

/**
 * @brief Initialize date display and start update task
 * @param date_label Pointer to the LVGL label object for date (DD/MM/YYYY format)
 */
void date_display_init(lv_obj_t *date_label);

/**
 * @brief Update the time label with current time
 */
void time_display_update(void);

/**
 * @brief Stop the time display task
 */
void time_display_stop(void);

#endif // TIME_DISPLAY_H
