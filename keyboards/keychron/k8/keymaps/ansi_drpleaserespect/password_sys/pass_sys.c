/* Copyright 2022 DrPleaseRespect <DrPleaseRespect@outlook.com>
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

#include "quantum.h"
#include "pass_sys.h"
#include "lib/lib8tion/lib8tion.h"

#define PASS_NUM 8
#define PASS_SIZE sizeof(pass_code_sys) / sizeof(pass_code_sys[0])
#define PassIndicatorSTART 17

// LOCKED = TRUE | UNLOCKED = FALSE
#define DefaultLockState true

static const uint16_t pass_code_sys[] = {KC_UP, KC_UP, KC_DOWN, KC_DOWN, KC_LEFT, KC_RIGHT, KC_LEFT, KC_RIGHT};
static int8_t passindex = 0;
static uint16_t passcode_history_sys[PASS_SIZE] = {0};
static bool pass_sys_unlocked = !DefaultLockState;

__attribute__ ((weak))
uint32_t get_millisecond_timer(void) {
  return timer_read32();
}

void pass_set(uint16_t keycode) {
	switch (keycode) {
		case KC_ENTER:
		{
			verify_pass();
			return;
		}
		case KC_BACKSPACE:
		{
			pass_remove();
			return;
		}
	}
	if (passindex < PASS_SIZE) {
		passcode_history_sys[passindex] = keycode;
		passindex++; 
	}
}

void pass_remove(void) {
	if (passindex > 0) {
		passindex--;
	}
	passcode_history_sys[passindex] = 0;
}

void display_pass_index(void) {
	if (!pass_sys_unlocked) {
		uint8_t beat_saw = beat8(255, 0);
		rgb_matrix_set_color_all(beat_saw,0,0);
	}
	
	for (uint16_t index = PassIndicatorSTART; index < (PassIndicatorSTART + passindex); ++index) {
		if (pass_sys_unlocked) {
			rgb_matrix_set_color(index, 0, 255, 0);
		}
		else {
			rgb_matrix_set_color(index, 0, 0, 255);
		}
	}
}

void reset_pass(void) {
	for (uint8_t index = 0; index < PASS_SIZE; ++index) {
		passcode_history_sys[index] = 0;
	}
	passindex = 0;
}

void lock_pass(void) {
	pass_sys_unlocked = false;
	reset_pass();
}

bool verify_pass(void) {
	for (uint8_t index = 0; index < PASS_SIZE; ++index) {
		if (passcode_history_sys[index] == pass_code_sys[index]) {
			continue;
		}
		else {
			reset_pass();
			pass_sys_unlocked = false;
			return false;
		}
	}
	pass_sys_unlocked = true;
	return true;
}

bool pass_sys_isunlocked(void) {
	return pass_sys_unlocked;
}

bool pass_sys_islocked(void) {
	return !pass_sys_unlocked;
}