/* Copyright 2020 Adam Honse <calcprogrammer1@gmail.com>
 * Copyright 2020 Dimitris Mantzouranis <d3xter93@gmail.com>
 * Copyright 2022 Harrison Chan (Xelus)
 * Copyright 2022 Thomas Bowman Mørch (tbowmo)
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

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layer_names {
    WIN_BASE    = 0,
    WIN_FN      = 1,
    MAC_BASE    = 2,
    MAC_FN      = 3,
};

#define KC_TASK LGUI(KC_TAB)        // Task viewer
#define KC_FLXP LGUI(KC_E)          // Windows file explorer
#define KC_CRTN LGUI(KC_C)          // Cortana | Microsoft Teams

#define KC_MSSN LGUI(KC_F3)         // Mission Control
#define KC_FIND LALT(LGUI(KC_SPC))  // Finder
#define KC_SIRI LGUI(KC_SPC)        // Siri
#define KC_MSNP LSFT(LGUI(KC_4))    // Mac snip tool

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*	Windows layout
    +--------------------------------------------------------------------------+----------------+
    | ESC |  | F1 | F2 | F3 | F4 | | F5 | F6 | F7 | F8 | | F9| F10| F11| F12|  | |PSCR|????|PAUS|
    +--------------------------------------------------------------------------+------|----|----|
    |  ~  |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 |  0 |  - |  = | BACKSP | |INS |SCRL|PGUP|
    +--------------------------------------------------------------------------+------|----|----|
    |  TAB  |  Q |  W |  E |  R |  T |  Y |  U |  I |  O |  P |  [ |  ] |   \  | |DEL |END |PGDN|
    +--------------------------------------------------------------------------+------|----|----|
    | CAPSLCK  |  A |  S |  D |  F |  G |  H |  J |  K |  L | ; | ' |  RETURN  |                |
    +--------------------------------------------------------------------------+      |----|    |
    | LSHIFT     |  Z |  X |  C |  V |  B |  N |  M | , | . |  / |   RSHIFT    |      | UP |    |
    +--------------------------------------------------------------------------+------|----|----|
    |LCTRL| LGUI| LALT |            SPACE            | RALT| RGUI | FN | RCTRL | |LFT |DWN |RGT |
    +--------------------------------------------------------------------------+----------------+
*/

    [WIN_BASE] = LAYOUT_tkl_ansi(
        KC_ESC,           KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,    KC_F12,      KC_PSCR,  KC_VOLD,  KC_VOLU,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,    KC_BSPC,     KC_INS,   KC_HOME,  KC_PGUP,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,   KC_BSLS,     KC_DEL,   KC_END,   KC_PGDN,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,            KC_ENT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT,                          KC_UP,
        KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                                      KC_RALT, KC_RGUI, MO(WIN_FN),KC_RCTL,     KC_LEFT,  KC_DOWN,  KC_RGHT
    ),

  [WIN_FN] = LAYOUT_tkl_ansi(
          RESET,          NK_TOGG, _______, _______, _______, _______, _______, KC_BRID, KC_BRIU, _______, _______, _______,   KC_MSTP,     KC_MPLY,  KC_MPRV,  KC_MNXT,
        _______, _______, KC_NUM , KC_PSLS, KC_PAST, _______, _______, _______, _______, _______, _______, _______, _______,   RGB_TOG,     RGB_SPI,  RGB_SAI,  RGB_HUI,
        _______, KC_KP_7, KC_KP_8, KC_KP_9, KC_PMNS, _______, _______, _______, _______, _______, _______, _______, _______,   _______,     RGB_SPD,  RGB_SAD,  RGB_HUD,
        _______, KC_KP_4, KC_KP_5, KC_KP_6, KC_PPLS, _______, _______, _______, _______, _______, _______, _______,            _______,
        _______, KC_KP_1, KC_KP_2, KC_KP_3, KC_PENT, _______, _______, _______, _______, _______, _______,          _______,                          RGB_VAI,
        _______, KC_PDOT, KC_KP_0,                   _______,                                     _______, _______, _______,   _______,     RGB_RMOD, RGB_VAD,  RGB_MOD
    ),

    /*  Mac layout
    +-------------------------------------------------------------------------------------------+
    | ESC |  | F1 | F2 | F3 | F4 | | F5 | F6 | F7 | F8 | | F9| F10| F11| F12|  | |PSCR|SIRI|RGB |
    +--------------------------------------------------------------------------+ +--------------+
    |  ~  |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 |  0 |  - |  = | BACKSP | |INS |HOME|PGUP|
    +--------------------------------------------------------------------------+ +--------------+
    |  TAB  |  Q |  W |  E |  R |  T |  Y |  U |  I |  O |  P |  [ |  ] |   \  | |DEL |END |PGDN|
    +--------------------------------------------------------------------------+ +--------------+
    | CAPSLCK  |  A |  S |  D |  F |  G |  H |  J |  K |  L | ; | ' |  RETURN  |                |
    +--------------------------------------------------------------------------+      +----+    |
    | LSHIFT     |  Z |  X |  C |  V |  B |  N |  M | , | . |  / |   RSHIFT    |      | UP |    |
    +--------------------------------------------------------------------------+ +--------------+
    |LCTRL| LALT| LGUI |            SPACE            | RGUI| RALT | FN | RCTRL | |LFT |DWN |RGT |
    +-------------------------------------------------------------------------------------------+
    */

  [MAC_BASE] = LAYOUT_tkl_ansi( // Mac base layout
      KC_ESC,           KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,    KC_F12,        KC_MSNP, KC_SIRI, RGB_MOD,
      KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,    KC_BSPC,       KC_INS,  KC_HOME, KC_PGUP,
      KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,   KC_BSLS,       KC_DEL,  KC_END,  KC_PGDN,
      KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,            KC_ENT,
      KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT,                           KC_UP,
      KC_LCTL, KC_LALT, KC_LGUI,                   KC_SPC,                                      KC_RGUI, KC_RALT, MO(MAC_FN),KC_RCTL,       KC_LEFT, KC_DOWN, KC_RGHT
  ),

  [MAC_FN] = LAYOUT_tkl_ansi( // Mac Fn overlay
      RESET,            KC_BRID, KC_BRIU, KC_MSSN, KC_FIND, RGB_VAD, RGB_VAI, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU,         _______, _______, RGB_TOG,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,         _______, _______, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,         _______, _______, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,                           _______,
      _______, _______, _______,                   _______,                                     _______, _______, _______, _______,         RGB_SAD, _______, RGB_SAI
    )
};

bool dip_switch_update_user(uint8_t index, bool active){
  switch(index){
    case 0:
      if(active) { // Mac mode
          //layer_move(MAC_BASE); Disabled because I don't want this
            layer_move(WIN_FN);
      } else { // Windows mode
          layer_move(WIN_BASE);
      }
      return false;
    case 1:
      if(active){ //Cable mode
        // do stuff
      }
      else{ //BT mode
        // do stuff
      }
      break;
  }

  return true;
}

void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    bool caps_override = false;
    if (IS_LAYER_ON(WIN_FN)) {
      caps_override = true;
        uint8_t layer = get_highest_layer(layer_state);

        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];

                if (index >= led_min && index <= led_max && index != NO_LED &&
                keymap_key_to_keycode(layer, (keypos_t){col,row}) > KC_TRNS) {
                    rgb_matrix_set_color(index, RGB_GREEN);
                }
            }
        }
    }
    else {
      caps_override = false;
    }
    if (caps_override == false) {
      if (host_keyboard_led_state().caps_lock) {
          for (uint8_t i = led_min; i <= led_max; i++) {
              if (g_led_config.flags[i] & LED_FLAG_KEYLIGHT) {
                  rgb_matrix_set_color(i, RGB_GOLD);
              }
          }
      }
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case WIN_FN:
        if (!host_keyboard_led_state().num_lock) {
             tap_code16(KC_NLCK);
        }
        break;
    default: //  for any other layers, or the default layer
        if (host_keyboard_led_state().num_lock) {
             tap_code16(KC_NLCK);
        }
        break;
    }
  return state;
}

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  //debug_enable=true;
  //debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
}
