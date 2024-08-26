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
#include "print.h"
#include "socd.h"

// Simultaneous Opposing Inputs
__attribute__ ((weak)) key_pair_state_t key_pairs[KEY_PAIR_NUM] = {};

#if KEY_PAIR_NUM < 1
#error "KEY_PAIR_NUM Needs to be greater than 0"
#endif

bool key_pair_cancellation_active = false;

void key_pair_cancellation_toggle(void) {
    key_pair_cancellation_active = !key_pair_cancellation_active;
}

void key_pair_cancellation_enable(void) {
    key_pair_cancellation_active = true;
}

void key_pair_cancellation_disable(void) {
    key_pair_cancellation_active = false;
}

bool key_pair_cancellation_is_enabled(void) {
    return key_pair_cancellation_active;
}

void key_pair_cancellation_rgb(uint16_t toggle_key, uint16_t keycode, uint8_t index) {
    if (keycode != 0) {
        if (keycode == toggle_key) {
            if (key_pair_cancellation_is_enabled()) {
                rgb_matrix_set_color(index, RGB_GREEN);
            } else {
                rgb_matrix_set_color(index, RGB_RED);
            }
        }        
    }
    if (key_pair_cancellation_active) {
        for (int i = 0; i < (sizeof(key_pairs) / sizeof(key_pairs[0])); ++i)
        {
            key_pair_state_t* key_pair = &key_pairs[i];

            if (key_pair->key1_pressed && !(key_pair->key2_pressed)) {
                if (keycode == key_pair->key1) {
                    rgb_matrix_set_color(index, RGB_GREEN);
                }
                if (keycode == key_pair->key2) {
                    rgb_matrix_set_color(index, RGB_RED);
                }

            }
            if (key_pair->key2_pressed && !(key_pair->key1_pressed)) {
                if (keycode == key_pair->key2) {
                    rgb_matrix_set_color(index, RGB_GREEN);
                }
                if (keycode == key_pair->key1) {
                    rgb_matrix_set_color(index, RGB_RED);
                }
            }

            if (key_pair->key2_pressed && key_pair->key1_pressed) {
                if (keycode == key_pair->key1) {
                    rgb_matrix_set_color(index, RGB_YELLOW);
                }
                if (keycode == key_pair->key2) {
                    rgb_matrix_set_color(index, RGB_YELLOW);
                }
            }
        }
    }
}

void key_pair_cancellation_handler(uint16_t toggle_key, uint16_t keycode, keyrecord_t *record) {
    if (toggle_key != 0) {
        if (keycode == toggle_key && record->event.pressed) {
            key_pair_cancellation_toggle();
        }
    }
    if (key_pair_cancellation_active && keycode != KC_NO) {
        for (int i = 0; i < (sizeof(key_pairs) / sizeof(key_pairs[0])); ++i)
        {
            key_pair_state_t* key_pair = &key_pairs[i];
            if (keycode == key_pair->key1) {
                if (key_pair->key2_pressed) {
                    if (record->event.pressed) {
                        unregister_code(key_pair->key2);                       
                    } else {
                        register_code(key_pair->key2);
                    }
                }
                key_pair->key1_pressed = record->event.pressed;
            }
            else if (keycode == key_pair->key2) {
                if (key_pair->key1_pressed) {
                    if (record->event.pressed) {
                        unregister_code(key_pair->key1);
                    } else {
                        register_code(key_pair->key1);
                    }
                }
                key_pair->key2_pressed = record->event.pressed;
            }
        }
    }
}