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

#pragma once

typedef struct
{
    bool key1_pressed;
    bool key2_pressed;
    uint8_t key1;
    uint8_t key2;
} key_pair_state_t;

#define KEY_PAIR_CANCELLATION_ENABLED

#define KEY_PAIR(key1, key2) {false, false, key1, key2}

// Adjust KEY_PAIR_NUM to the desired number of pairs
#define KEY_PAIR_NUM 2
extern key_pair_state_t key_pairs[KEY_PAIR_NUM];

void key_pair_cancellation_toggle(void);
void key_pair_cancellation_enable(void);
void key_pair_cancellation_disable(void);
bool key_pair_cancellation_is_enabled(void);
void key_pair_cancellation_rgb(uint16_t toggle_key, uint16_t keycode, uint8_t index);
void key_pair_cancellation_handler(uint16_t toggle_key, uint16_t keycode, keyrecord_t *record);
