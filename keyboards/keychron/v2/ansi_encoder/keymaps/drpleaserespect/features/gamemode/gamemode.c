/* Copyright 2023 DrPleaseRespect <DrPleaseRespect@outlook.com>
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
#include "lib/lib8tion/lib8tion.h"
#include "gamemode.h"

bool gm_active = false;

// lib8tion get_millisecond_timer
__attribute__ ((weak)) uint32_t get_millisecond_timer(void) {
  return timer_read32();
}

bool process_gm_hook(uint16_t gm_keycode, uint16_t keycode, keyrecord_t *record) {
	if (keycode == gm_keycode) {
		if (record->event.pressed) {
			gm_active = !gm_active;	
		}
		return false;
	} else if (keycode == KC_LGUI || keycode == KC_RGUI) {
		return !gm_active;
	} else {
		return true;
	}
}

void rgb_matrix_gm_hook(uint16_t gm_keycode, uint16_t keycode, uint8_t index) {
	if (keycode == gm_keycode) {
		if (gm_active) {
			rgb_matrix_set_color(index, RGB_GREEN);				
		} else {
			rgb_matrix_set_color(index, RGB_RED);
		}
	} else if (keycode == KC_LGUI || keycode == KC_RGUI) {
		if (gm_active) {
			rgb_matrix_set_color(index, beatsin8(100, 0, 255, 0, 0) ,0,0);				
		}
	}
}