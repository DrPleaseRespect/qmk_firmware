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
#include "lib/lib8tion/lib8tion.h"
#include "encoder_spinner.h"
#include "print.h"


static uint8_t spinner_position = 0;
static bool	spinner_active   = false;
static bool spinner_dtapped  = false;

static bool spinner_tapped = false;
static uint16_t spin_tap_timer = 0;


// lib8tion get_millisecond_timer
__attribute__ ((weak)) uint32_t get_millisecond_timer(void) {
  return timer_read32();
}

__attribute__ ((weak)) void process_encoder_keys(uint8_t pos, uint8_t flags) {
	// Processing of Encoder
	dprintf("encoder_spinner pos: %x \n", flags);
}


void spinner_key(uint16_t CW_keycode, uint16_t CCW_keycode, uint16_t buttonkeycode, uint16_t keycode, keyrecord_t *record) {
	if (record->event.pressed) {
		// Spinner Button
		if (keycode == buttonkeycode) {
			if (spinner_tapped && !timer_expired(record->event.time, spin_tap_timer)) {
				// Double Tapped
				spinner_dtapped = true;
				spinner_active = !spinner_active;
				dprint("Spinner Key Double Tapped \n");
				dprintf("spinner_active Status: %hu \n", spinner_active);
			}
			spinner_tapped = true;
			spin_tap_timer = record->event.time + SPIN_DOUBLETAP_DELAY;
		} else {
			spinner_tapped = false;
			spinner_dtapped = false;
		}

		// Counterclock-wise Function
		if (keycode == CCW_keycode) {
			if (spinner_active) {
				spinner_position = spinner_position - 1;
				if (spinner_position >= SPINNER_SIZE) {
					spinner_position = SPINNER_SIZE - 1; // Remove 1 for Zero Indexing
				}
				dprintf("Spinner Position: %hu \n", spinner_position);
			} else {
				process_encoder_keys(spinner_position, SPINNER_CCW);
			}
		}


		// Clockwise Function
		if (keycode == CW_keycode) {
			if (spinner_active) {
				spinner_position = spinner_position + 1;
				if (spinner_position >= SPINNER_SIZE) {
					spinner_position = 0;
				}
				dprintf("Spinner Position: %hu \n", spinner_position);
			} else {
				process_encoder_keys(spinner_position, SPINNER_CW);
			}
		}
	}
}

void matrix_scan_spinner(void) {
	// Matrix Scan for Delay
	// I don't want to use a Deferred executor tbh

	// Single Tap Spinner Function
	if (spinner_tapped) {
		if (timer_expired(timer_read(), spin_tap_timer)) {
			if (!spinner_dtapped) {
				spinner_tapped = false; // Reset State
				dprint("Spinner Key Single Tapped \n");
				if (!spinner_active) {
					process_encoder_keys(spinner_position, SPINNER_BUTTON);
				} else {
					spinner_active = !spinner_active;
				}
			} else {
				spinner_dtapped = false;
				spinner_tapped = false;
			}

		}
	}
}



void spinner_rgb_indicator(void) {
	for (uint8_t index = SPIN_INDI_INDEX;
		 index <= (SPIN_INDI_INDEX + (SPINNER_SIZE - 1)); // SPINNER_SIZE - 1 // For 0 Index
		  ++index) {
		if ((spinner_position + SPIN_INDI_INDEX) == index) {
			if (spinner_active) {
				rgb_matrix_set_color(index, 0,beatsin8(100, 0, 255, 0, 0) ,0);
			} else {
				rgb_matrix_set_color(index, RGB_GREEN);

			}
		} else {
		rgb_matrix_set_color(index, RGB_RED);			
		}
	}
}