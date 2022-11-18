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

#define PASS_SIZE sizeof(pass_code) / sizeof(pass_code[0])
#define PassIndicatorSTART 17

// LOCKED = TRUE | UNLOCKED = FALSE
#define DefaultLockState false

typedef union {
	uint32_t raw;
	struct {
		bool pass_sys_unlocked :1;
	};
} user_config_t;

user_config_t eeprom_config;

static const uint16_t pass_code[] = {KC_UP, KC_UP, KC_DOWN, KC_DOWN, KC_LEFT, KC_RIGHT, KC_LEFT, KC_RIGHT};
static int8_t passindex = 0;
static uint16_t passcode_history_sys[PASS_SIZE] = {0};


// PASS MANAGEMENT
void pass_set(uint16_t keycode) {
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

void reset_pass(void) {
	for (uint8_t index = 0; index < PASS_SIZE; ++index) {
		passcode_history_sys[index] = 0;
	}
	passindex = 0;
}

bool verify_pass(void) {
	for (uint8_t index = 0; index < PASS_SIZE; ++index) {
		if (passcode_history_sys[index] == pass_code[index]) {
			continue;
		}
		else {
			reset_pass();
			eeprom_config.pass_sys_unlocked = false;
			return false;
		}
	}
	eeprom_config.pass_sys_unlocked = true;
	if (DefaultLockState == false) {
		eeconfig_update_user(eeprom_config.raw);
	}
	return true;
}

// HOOKS

bool pass_hook(keyrecord_t *record) {
	// BYPASS LAYERS AND ONLY USE KEYCODES AVAILABLE IN LAYER 0
	uint16_t keycode = keymap_key_to_keycode(0, record->event.key);
	if (pass_sys_isunlocked() == false) {
		if (record->event.pressed) {
			switch (keycode) {
				case KC_ENTER:
				{
					verify_pass();
					break;
				}
				case KC_BACKSPACE:
				{
					pass_remove();
					break;
				}
				default:
				{
					pass_set(keycode);
					break;
				}
			}
			
			return false;
		}
	}
	return true;
}

// DISPLAY HOOK
void display_pass_index(void) {
	static uint16_t pass_indicator = 0;
	static bool display = false;
	
	if (!eeprom_config.pass_sys_unlocked) {
		if (pass_indicator != 0) {
			pass_indicator = 0;
		}
		display = true;
		uint8_t beat_saw = beat8(255, 0);
		rgb_matrix_set_color_all(beat_saw,0,0);
	}
	else {
		if (pass_indicator == 0) {
			pass_indicator = timer_read();
		}
	}

	if ((timer_elapsed(pass_indicator) < 2000 || !eeprom_config.pass_sys_unlocked) && display) {
		for (uint8_t index = PassIndicatorSTART; index < (PassIndicatorSTART + passindex); ++index) {
			if (eeprom_config.pass_sys_unlocked) {
				rgb_matrix_set_color(index, 0, 255, 0);
			}
			else {
				rgb_matrix_set_color(index, 0, 0, 255);
			}
		}
	}
	else {
		display = false;
		return;
	}
}

void pass_sys_kb_postinit_hook(void) {
	eeprom_config.raw = eeconfig_read_user();
}

void pass_sys_eeconfig_init_hook(void) {
	eeprom_config.raw = 0;
	eeprom_config.pass_sys_unlocked = !DefaultLockState;
	eeconfig_update_user(eeprom_config.raw);
}

// HELPER FUNCTIONS

void lock_pass(void) {
	eeprom_config.pass_sys_unlocked = false;
	if (DefaultLockState == false) {
		eeconfig_update_user(eeprom_config.raw);
	}
	reset_pass();
}

bool pass_sys_isunlocked(void) {
	return eeprom_config.pass_sys_unlocked;
}

bool pass_sys_islocked(void) {
	return !eeprom_config.pass_sys_unlocked;
}


// weak functions

__attribute__ ((weak))
uint32_t get_millisecond_timer(void) {
  return timer_read32();
}

__attribute__ ((weak))
void eeconfig_init_user(void) {
	pass_sys_eeconfig_init_hook();
}

__attribute__ ((weak))
void keyboard_post_init_user(void) {
	pass_sys_kb_postinit_hook();
}