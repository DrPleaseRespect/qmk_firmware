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
#include "os_detection.h"
#include "color.h"

#define AUTONKRO_DISPLAY_DURATION 1000

static bool os_debounce_finished = false;

static uint16_t autonkro_indicator_timer = 0;

static os_variant_t os_detected = OS_UNSURE;
static bool autonkro_indicators_finished = false;

bool process_autonkro_host_os(os_variant_t detected_os) {
    switch (detected_os) {
        case OS_MACOS:
        case OS_IOS:
            keymap_config.nkro = false;
            break;
        case OS_WINDOWS:
            keymap_config.nkro = true;
            break;
        case OS_LINUX:
            keymap_config.nkro = true;
            break;
        case OS_UNSURE:
            keymap_config.nkro = false;
            break;
    }

    clear_keyboard();
    os_debounce_finished = true;
    os_detected = detected_os;
    return true;
}

void autonkro_indicators(void) {
    if (autonkro_indicators_finished) {
        return;
    }
    if (os_debounce_finished && autonkro_indicator_timer == 0) {
        autonkro_indicator_timer = timer_read() + AUTONKRO_DISPLAY_DURATION;
    }
    if (autonkro_indicators_finished == false) {
        uint16_t blink_timer = autonkro_indicator_timer - (AUTONKRO_DISPLAY_DURATION / 2);

        if (!timer_expired(timer_read(), autonkro_indicator_timer) || autonkro_indicator_timer == 0) {
            if (!timer_expired(timer_read(), blink_timer) || autonkro_indicator_timer == 0) {
                switch (os_detected) {
                    case OS_LINUX:
                        rgb_matrix_set_color_all(RGB_YELLOW);
                        break;
                    case OS_WINDOWS:
                        rgb_matrix_set_color_all(RGB_BLUE);
                        break;
                    case OS_MACOS:
                    case OS_IOS:
                        rgb_matrix_set_color_all(RGB_WHITE);
                        break;
                    case OS_UNSURE:
                    default:
                        rgb_matrix_set_color_all(RGB_ORANGE);
                        break;
                }
            } else {
                if ((timer_elapsed(blink_timer) % 200) < 100) {
                        if (keymap_config.nkro) {
                            rgb_matrix_set_color_all(RGB_GREEN);                            
                        } else {
                            rgb_matrix_set_color_all(RGB_RED);                            
                        }
                } else {
                        rgb_matrix_set_color_all(RGB_BLACK);
                }
            }

        } else {
            autonkro_indicators_finished = true;
        }
    }
}