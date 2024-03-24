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

#define SPIN_INDI_INDEX 1
#define SPIN_DOUBLETAP_DELAY 300

#define SPINNER_SIZE 5

#define SPINNER_CW     (1 << 0)
#define SPINNER_CCW    (1 << 1)
#define SPINNER_BUTTON (1 << 2)


void process_encoder_keys(uint8_t pos, uint8_t flags);

void spinner_key(uint16_t CW_keycode, uint16_t CCW_keycode, uint16_t buttonkeycode, uint16_t keycode, keyrecord_t *record);

void matrix_scan_spinner(void);

void spinner_rgb_indicator(void);