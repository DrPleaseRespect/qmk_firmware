/* Copyright (C) 2023 Benjamín Ausensi
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_60_ansi(
        KC_ESC,     KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       KC_MINS,    KC_EQL,     KC_BSPC,    
        KC_TAB,     KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,       KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_LBRC,    KC_RBRC,    KC_BSLS,    
        KC_CAPS,    KC_A,       KC_S,       KC_D,       KC_F,       KC_G,       KC_H,       KC_J,       KC_K,       KC_L,       KC_SCLN,    KC_QUOT,                KC_ENT,     
        KC_LSFT,                KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,       KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,    KC_RSFT,                
        KC_LCTL,    KC_LALT,    KC_LCMD,                                        KC_SPC,                             KC_RCMD,    KC_RALT,    KC_RCTL,                MO(1)
    ),
    [1] = LAYOUT_60_ansi(
        KC_GRV,     KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,     KC_F12,     KC_DEL,    
        KC_TRNS,    KC_F13,     KC_F14,     KC_F15,     KC_F16,     KC_F17,     KC_PSCR,    KC_SCRL,    KC_PAUS,    RGB_HUD,    RGB_HUI,    RGB_VAD,    RGB_VAI,    RGB_MOD,   
        KC_TRNS,    AG_SWAP,    AG_NORM,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_INS,     KC_HOME,    KC_PGUP,    KC_TRNS,    RGB_SPD,    RGB_SPI,                KC_TRNS,   
        KC_TRNS,                KC_MUTE,    KC_VOLD,    KC_VOLU,    KC_MPRV,    KC_MNXT,    KC_DEL,     KC_END,     KC_PGDN,    RGB_HUI,    KC_UP,      KC_TRNS,   
        QK_BOOT,    KC_TRNS,    KC_TRNS,                                        KC_TRNS,                            KC_LEFT,    KC_DOWN,    KC_RIGHT,               KC_TRNS
    )
};

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (host_keyboard_led_state().caps_lock) {
        // Set capslock key to orange (capslock is led number 30)
        rgb_matrix_set_color(28, 255, 130, 15);
    }
    return false;
}
