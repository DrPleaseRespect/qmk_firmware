/* Copyright 2024 DrPleaseRespect <DrPleaseRespect@outlook.com>
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "rgb_control_status.h"
#include <math.h>

static uint16_t display_until = 0;
static bool rgb_status_active = false;
static bool wide_status = false;
static bool rgb_status_max = 0;
static bool rgb_status_value = 0;

void display_status(uint8_t value, uint8_t max_value, bool wide) {
    display_until = timer_read() + RGBSTATUS_DURATION;
    rgb_status_active = true;
    wide_status = wide;
    rgb_status_value = value;
    rgb_status_max = max_value;
}

void display_rgb_mode(void) {
    uint8_t current_mode = rgb_matrix_get_mode();
    uint8_t max_modes = RGB_MATRIX_EFFECT_MAX - 1; // Max Value refers to the last value

    display_status(current_mode, max_value, true);

}

void display_rgb_val(void) {
    uint8_t current_status = rgb_matrix_get_val();
    display_status(current_status, 255, false);
}

void display_rgb_hue(void) {
    uint8_t current_status = rgb_matrix_get_hue();
    display_status(current_status, 255, false);
}

void display_rgb_sat(void) {
    uint8_t current_status = rgb_matrix_get_sat();
    display_status(current_status, 255, false);
}

void display_rgb_speed(void) {
    uint8_t current_status = rgb_matrix_get_speed();
    display_status(current_status, 255, false);
}

void stop_rgb_status(void) {
    // RESET VALUES
    display_until = 0;
    rgb_status_active = false;
    wide_status = false;
    rgb_status_value = 0;
    rgb_status_max = 0;
}

void matrix_rgb_indicator(void) {
    // TODO: RGB MATRIX LOGIC (DISPLAY LOGIC)
    if (rgb_status_active) {
        if (wide_status) {
            // Wide Display

        } else {
            // Non-Wide Display
        }
    }

}

void matrix_scan_rgbstatus(void) {
    // Matrix Scan for Delay

    if (timer_expired(timer_read(), display_until)) {
        stop_rgb_status();
    }
}