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
#include "lib/lib8tion/lib8tion.h"

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layer_names {
    WIN_BASE    = 0,
    WIN_FN      = 1,
    MACRO       = 2,
};

enum custom_keycodes {
  GM_MODE = SAFE_RANGE,
  M_SHUT,
  S_CADET,
};


bool init_eeprom = false; // Only Sets to True if EEPROM has been Reset
bool Macro_Active = false;
bool rgb_enabled = true;
bool gui_keys_enabled = true;
bool space_cadet_keys = false;


uint16_t blink_timer;
uint16_t Macro_Timer = 0; // Initialize Macro Timer Used for Macro Time Delayed Functions
uint16_t macro_keycode; // Keycode of Macro


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
        KC_LSPO, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSPC,                          KC_UP,
        KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                                      KC_RALT, KC_RGUI, MO(WIN_FN),KC_RCTL,     KC_LEFT,  KC_DOWN,  KC_RGHT
    ),

  [WIN_FN] = LAYOUT_tkl_ansi(
          RESET,          NK_TOGG, GM_MODE, S_CADET, _______, _______, _______, KC_BRID, KC_BRIU, _______, _______, _______,   KC_MSTP,     KC_MPLY,  KC_MPRV,  KC_MNXT,
        _______, _______, KC_NUM , KC_PSLS, KC_PAST, _______, _______, _______, _______, _______, _______, _______, _______,   _______,     RGB_SPI,  RGB_SAI,  RGB_HUI,
        _______, KC_KP_7, KC_KP_8, KC_KP_9, KC_PMNS, _______, _______, _______, _______, _______, _______, _______, _______,   RGB_TOG,     RGB_SPD,  RGB_SAD,  RGB_HUD,
        _______, KC_KP_4, KC_KP_5, KC_KP_6, KC_PPLS, _______, _______, _______, _______, M_SHUT , _______, _______,            _______,
        _______, KC_KP_1, KC_KP_2, KC_KP_3, KC_PENT, _______, _______, _______, _______, _______, _______,         MO(MACRO),                         RGB_VAI,
        _______, KC_PDOT, KC_KP_0,                   _______,                                     _______, _______, XXXXXXX,   _______,     RGB_RMOD, RGB_VAD,  RGB_MOD
    ),
  [MACRO] = LAYOUT_tkl_ansi(
         XXXXXXX,          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX,  XXXXXXX,  XXXXXXX,
         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX,  XXXXXXX,  XXXXXXX,
         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX,  XXXXXXX,  XXXXXXX,
         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,
         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,                         XXXXXXX,
         XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX,                                     XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX,     XXXXXXX, XXXXXXX,  XXXXXXX
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
};
// get_millisecond_timer function for lib8tion
uint32_t get_millisecond_timer(void) {
  return timer_read32();
}

void disable_rgb_untracked(bool status) {
  static HSV RGB_HISTORY_HSV;
  static uint8_t RGB_HISTORY_MODE;
  if (status) {
    RGB_HISTORY_HSV = rgb_matrix_get_hsv();
    RGB_HISTORY_MODE = rgb_matrix_get_mode();
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv_noeeprom(HSV_OFF);
  }
  else {
    HSV colors = RGB_HISTORY_HSV;
    rgb_matrix_mode_noeeprom(RGB_HISTORY_MODE);
    rgb_matrix_sethsv_noeeprom(colors.h, colors.s, colors.v);
  }
}

void disable_rgb_tracked(bool status) {
  if (status) {
    rgb_enabled = false;
    disable_rgb_untracked(status);
  }
  else {
    rgb_enabled = true;
    disable_rgb_untracked(status);
  }
}

// TOGGLE FOR SPACE CADET KEY
bool space_cadet_key_function(uint16_t keycode, bool pressed){
    if (space_cadet_keys) {
      return true;
    }

    // MAIN LOGIC
    if (pressed) {
      switch(keycode) {
        case KC_LSPO:
          register_code(KC_LSFT);
        case KC_RSPC:
          register_code(KC_RSFT);
      }
    }
    else {
      switch(keycode) {
        case KC_LSPO:
          unregister_code(KC_LSFT);
        case KC_RSPC:
          unregister_code(KC_RSFT);
      }
    }
    return false;
}

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

void matrix_status_indicators(void) {
  // -SECTION START- NKRO INDICATOR
  if (keymap_config.nkro) {
    rgb_matrix_set_color(1, RGB_BLUE);
  }
  else {
    rgb_matrix_set_color(1, RGB_RED);
  }
  // -SECTION END-

  // -SECTION START- GAME MODE WINDOWS KEY INDICATORS
  // AND SPACE CADET KEY INDICATORS

  uint8_t beat_sin = beatsin8(100, 0, 255, 0, 0); // 100BPM Sine Wave Generator (8-bit)
  
  if (!gui_keys_enabled || space_cadet_keys) {
    // I can make this better but for performance's sake i'll do it this way..
    uint8_t keys[4] = {77, 81, 63, 74}; // Specific Coordinates for K8
    uint8_t rows[4] = {5,5, 4, 4};    // Specific Coordinates for K8
    uint8_t col[4] = {1, 11, 0, 13};   // Specific Coordinates for K8
    uint8_t layer = get_highest_layer(layer_state);
    if (layer > 0) {
      for (uint8_t index = 0; index < 4; ++index) {
        uint16_t keycode = keymap_key_to_keycode(layer, (keypos_t){col[index],rows[index]});
        if ((keycode == KC_TRNS)) {
          // GAME MODE
          if (!gui_keys_enabled && index < 2){
            rgb_matrix_set_color(keys[index], beat_sin, 0, 0);
          }
          // SPACE CADET
          if (space_cadet_keys && index > 1){
            rgb_matrix_set_color(keys[index], beat_sin, 0, 0);
          }
        }
      }
    }
    else {
      for (uint8_t index = 0; index < 4; ++index) {
          // GAME MODE          
          if (!gui_keys_enabled && index < 2){
            rgb_matrix_set_color(keys[index], beat_sin, 0, 0);
          }
          // SPACE CADET
          if (space_cadet_keys && index > 1){
            rgb_matrix_set_color(keys[index], beat_sin, 0, 0);
          }
      }
    }
  }

  // -SECTION END-
}

void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    bool caps_override = false;
    uint8_t layer = get_highest_layer(layer_state);

    switch (layer) {
        case WIN_FN:
          caps_override = true;
          const uint16_t blocked_keys[] = {
            RESET, NK_TOGG, GM_MODE, M_SHUT, S_CADET,
          };
          const uint8_t blocked_keys_size = sizeof(blocked_keys) / sizeof(blocked_keys[0]);
          // -SECTION START- LAYER INDICATOR FOR CONFIGURED KEYS IN WIN_FN
          for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
              for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                  uint8_t index = g_led_config.matrix_co[row][col];
                  uint16_t keycode = keymap_key_to_keycode(layer, (keypos_t){col,row});
                  if (index >= led_min && 
                      index <= led_max && 
                      index != NO_LED &&
                      keycode > KC_TRNS) {
                        // DON'T SET RGB COLOR OF KEYS THAT ARE IN THE BLOCKED_KEYS ARRAY
                        bool blocked_key = false;
                        for (uint8_t block_index = 0; block_index < blocked_keys_size; ++block_index) {
                          if (blocked_keys[block_index] == keycode) {
                            blocked_key = true;
                          }
                        }
                        if (!blocked_key) {
                          rgb_matrix_set_color(index, RGB_GREEN);
                      }
                    }
                  }
              }
          // -SECTION END-

          // -SECTION START-  NUMLOCK INDICATOR
          if (host_keyboard_led_state().num_lock) {
            rgb_matrix_set_color(18, RGB_GREEN);
          }
          else {
            rgb_matrix_set_color(18, RGB_RED);
          }
          // -SECTION END-

          // -SECTION START-  BLINKING RGB LIGHTS
          if (timer_elapsed(blink_timer) >= 0 && timer_elapsed(blink_timer) <= 100) {
            rgb_matrix_set_color(0, RGB_RED);
          }
          else {
            if (timer_elapsed(blink_timer) >= 200) {
              blink_timer = timer_read();
            }
            rgb_matrix_set_color(0, RGB_BLACK);


          }
          uint8_t beat_sin = beatsin8(200, 0, 255, 0, 0); // 200BPM Sine Wave Generator (8-bit)
          // Game Mode Indicator
          if (gui_keys_enabled) { 
            rgb_matrix_set_color(2, beat_sin, 0, 0); // DISABLED COLOR: RED
          }
          else {
            rgb_matrix_set_color(2, 0, beat_sin, 0); // ENABLED COLOR: GREEN
          }

          // M SHUT MACRO
          rgb_matrix_set_color(59, beat_sin, 0, 0);

          // SPACE CADET KEY
          if (space_cadet_keys) {
            rgb_matrix_set_color(3, 0, beat_sin, 0); // ENABLED COLOR: GREEN
          }
          else {
            rgb_matrix_set_color(3, beat_sin, 0, 0); // DISABLED COLOR: RED
          }


          // -SECTION END-
          break;

        case MACRO:
          caps_override = true;
          // -SECTION START- LAYER INDICATOR FOR CONFIGURED KEYS IN MACRO
          for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
              for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                  uint8_t index = g_led_config.matrix_co[row][col];
                  uint16_t keycode = keymap_key_to_keycode(layer, (keypos_t){col,row});
                  if (index >= led_min && index <= led_max && index != NO_LED &&
                      keycode > KC_TRNS) {
                      switch(keycode) {
                        default:
                          rgb_matrix_set_color(index, RGB_RED);
                          break;
                      }
                  }
              }
          }
          // -SECTION END-
          break;

        default:
          break;
    }
    // -SECTION START- CAPS LOCK INDICATOR (HIGHLIGHT ALL ALPHA CHARACTERS)
    if (caps_override == false) {
      if (host_keyboard_led_state().caps_lock) {
          const uint8_t SIZE = 3;
          uint8_t start[3] = {34, 51, 64}; // Specific Coordinates for K8
          uint8_t end[3] = {43, 59, 70}; // Specific Coordinates for K8

          for (uint8_t i = 0; i < SIZE; i++) {
            for (uint8_t u = start[i]; u <= end[i]; u++){
              if (g_led_config.flags[u] & LED_FLAG_KEYLIGHT) {
                  rgb_matrix_set_color(u, RGB_GOLD);
              }
            }
          }
      }
    }
    // -SECTION END-

    // -SECTION START- CUSTOM MATRIX STATUS INDICATORS
    matrix_status_indicators();
    // -SECTION END-
}

void Macro_Initialize(uint16_t keycode) {
  macro_keycode = keycode;
  Macro_Active = true;
}

void Macro_Start_Timer(void) {
  Macro_Timer = timer_read();
}

void Macro_End(void) {
  Macro_Active = false;
  Macro_Timer = 0;
}

void Macro_functions(void) {
  // Function For Time Delayed Macros
  if (Macro_Active) {
    switch(macro_keycode) {
      case M_SHUT:
        if (timer_elapsed(Macro_Timer) > 300) {
          SEND_STRING("shutdown /s /t 0 /f /c deeznuts " SS_TAP(X_ENT));
          Macro_End();
          return;
        }
      default:
        return;
      }
    }
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    // RGB TOGGLE
    case RGB_TOG:
      if (record->event.pressed) {
        if (rgb_enabled) {
          disable_rgb_tracked(true);
        }
        else {
          disable_rgb_tracked(false);
        }
      }
      return false;
    // GAME MODE
    case KC_LGUI:
      return gui_keys_enabled;
    case KC_RGUI:
      return gui_keys_enabled;
    case GM_MODE:
      if (record->event.pressed) {
        if (gui_keys_enabled) {
          gui_keys_enabled = false;
        }
        else {
          gui_keys_enabled = true;
        }
      }
      return true;
    // MACROS
    case M_SHUT:
      if (record->event.pressed) {
        if (!Macro_Active) {
          Macro_Initialize(keycode);
          SEND_STRING(SS_LGUI("r"));
          Macro_Start_Timer();
        }
      }
      return true;
    // SPACE CADET SYSTEM
    case KC_LSPO:
      return space_cadet_key_function(keycode, record->event.pressed);
    case KC_RSPC:
      return space_cadet_key_function(keycode, record->event.pressed);
    case S_CADET:
      if (record->event.pressed) {
        unregister_code16(KC_LSPO);
        unregister_code16(KC_RSPC);
        if (space_cadet_keys) {
          space_cadet_keys = false;
        }
        else {
          space_cadet_keys = true;
        }
      }
      return true;
    default:
      return true;
  }
}



void matrix_scan_user(void) {
  //matrix_status_indicators();
  Macro_functions();

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

void keyboard_pre_init_user(void) {
  blink_timer = timer_read();
}

void eeconfig_init_user(void) {
  init_eeprom = true; // Enabled due to reset of EEPROM
}

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  //debug_enable=true;
  //debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
  if (init_eeprom) { // Only Execute if eeprom is reset
    rgb_matrix_enable();
    rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv(180, 255, 255);
  }
}
