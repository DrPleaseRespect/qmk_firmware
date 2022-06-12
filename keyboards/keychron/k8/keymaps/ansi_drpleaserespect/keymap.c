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
#include "raw_hid.h"
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
};

#ifdef RAW_ENABLE
enum raw_hid_functions {
  HID_SET_COLOR = 1,
  HID_SET_SPEED,
  HID_SET_MODE,
  HID_GET_AVAILABLE_MODES,
  HID_GET_CURRENT_RGB_SETTINGS,
  HID_SAVE_RGB_SETTINGS,
  HID_VOLUME,
};
#endif

#ifdef RAW_ENABLE
static const uint8_t rgb_matrix_effects_indexes[] = {
        1,  // SOLID COLOR
    #ifdef ENABLE_RGB_MATRIX_ALPHAS_MODS
        2,
    #endif
    #ifdef ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
        3,
    #endif
    #ifdef ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
        4,
    #endif
    #ifdef ENABLE_RGB_MATRIX_BREATHING
        5,
    #endif
    #ifdef ENABLE_RGB_MATRIX_BAND_SAT
        6,
    #endif
    #ifdef ENABLE_RGB_MATRIX_BAND_VAL
        7,
    #endif
    #ifdef ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
        8,
    #endif
    #ifdef ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
        9,
    #endif
    #ifdef ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT
        10,
    #endif
    #ifdef ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL
        11,
    #endif
    #ifdef ENABLE_RGB_MATRIX_CYCLE_ALL
        12,
    #endif
    #ifdef ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
        13,
    #endif
    #ifdef ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
        14,
    #endif
    #ifdef ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
        15,
    #endif
    #ifdef ENABLE_RGB_MATRIX_CYCLE_OUT_IN
        16,
    #endif
    #ifdef ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
        17,
    #endif
    #ifdef ENABLE_RGB_MATRIX_CYCLE_PINWHEEL
        18,
    #endif
    #ifdef ENABLE_RGB_MATRIX_CYCLE_SPIRAL
        19,
    #endif
    #ifdef ENABLE_RGB_MATRIX_DUAL_BEACON
        20,
    #endif
    #ifdef ENABLE_RGB_MATRIX_RAINBOW_BEACON
        21,
    #endif
    #ifdef ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
        22,
    #endif
    #ifdef ENABLE_RGB_MATRIX_RAINDROPS
        23,
    #endif
    #ifdef ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
        24,
    #endif
    #ifdef ENABLE_RGB_MATRIX_HUE_BREATHING
        25,
    #endif
    #ifdef ENABLE_RGB_MATRIX_HUE_PENDULUM
        26,
    #endif
    #ifdef ENABLE_RGB_MATRIX_HUE_WAVE
        27,
    #endif
    #ifdef ENABLE_RGB_MATRIX_PIXEL_RAIN
        28,
    #endif
    #ifdef ENABLE_RGB_MATRIX_PIXEL_FRACTAL
        29,
    #endif
    #if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP)
        30,
    #endif
    #if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_DIGITAL_RAIN)
        31,
    #endif
    #ifdef RGB_MATRIX_KEYREACTIVE_ENABLED
    #    ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
        32,
    #    endif
    #    ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE
        33,
    #    endif
    #    ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
        34,
    #    endif
    #    ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
        35,
    #    endif
    #    ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
        36,
    #    endif
    #    ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
        37,
    #    endif
    #    ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
        38,
    #    endif
    #    ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
        39,
    #    endif
    #    ifdef ENABLE_RGB_MATRIX_SPLASH
        40,
    #    endif
    #    ifdef ENABLE_RGB_MATRIX_MULTISPLASH
        41,
    #    endif
    #    ifdef ENABLE_RGB_MATRIX_SOLID_SPLASH
        42,
    #    endif
    #    ifdef ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
        43,
    #    endif
    #endif
};
#endif




bool init_eeprom = false; // Only Sets to True if EEPROM has been Reset
bool Macro_Active = false;
bool rgb_enabled = true;
bool gui_keys_enabled = true;
uint8_t SystemVolume;


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
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT,                          KC_UP,
        KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                                      KC_RALT, KC_RGUI, MO(WIN_FN),KC_RCTL,     KC_LEFT,  KC_DOWN,  KC_RGHT
    ),

  [WIN_FN] = LAYOUT_tkl_ansi(
          RESET,          NK_TOGG, GM_MODE, _______, _______, _______, _______, KC_BRID, KC_BRIU, _______, _______, _______,   KC_MSTP,     KC_MPLY,  KC_MPRV,  KC_MNXT,
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

void VolumeLevelIndicator(bool pressed){
    static uint16_t Volume_Timer = 0;
    uint8_t rgb_value;
    uint8_t id;
    #define VolumeSTART 17
    if (pressed == true){
        Volume_Timer = timer_read();
    }
    if (timer_elapsed(Volume_Timer) < 1000) {
        uint8_t normalized_volume = (uint16_t)(SystemVolume * 10) / 100;
        rgb_value = ((SystemVolume % 10) * 255) / (10 - 1);
        if (normalized_volume == 0) {
            id = 0 + VolumeSTART;
            rgb_matrix_set_color(id, 0, 0, rgb_value);

        }
        for (uint8_t index = 0; index < normalized_volume ; ++index) {
            id = index + VolumeSTART;
            if (index >= 9) {
                rgb_value = 255;
            }
            rgb_matrix_set_color(id, (255 - rgb_value), rgb_value, 0);
            //rgb_matrix_set_color(5, 255,255,255);

       }
    }
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

  VolumeLevelIndicator(false);

  // -SECTION START- NKRO INDICATOR
  if (keymap_config.nkro) {
    rgb_matrix_set_color(1, RGB_BLUE);
  }
  else {
    rgb_matrix_set_color(1, RGB_RED);
  }
  // -SECTION END-

  // -SECTION START- GAME MODE WINDOWS KEY INDICATORS
  uint8_t beat_sin = beatsin8(100, 0, 255, 0, 0); // 100BPM Sine Wave Generator (8-bit)

  if (!gui_keys_enabled) {
    // I can make this better but for performance's sake i'll do it this way..
    uint8_t keys[2] = {77, 81}; // Specific Coordinates for K8
    uint8_t rows[2] = {5,5};    // Specific Coordinates for K8
    uint8_t col[2] = {1, 11};   // Specific Coordinates for K8
    uint8_t layer = get_highest_layer(layer_state);
    if (layer > 0) {
      for (uint8_t index = 0; index < 2; ++index) {
        uint16_t keycode = keymap_key_to_keycode(layer, (keypos_t){col[index],rows[index]});
        if ((keycode == KC_TRNS)) {
          rgb_matrix_set_color(keys[index], beat_sin, 0, 0);
        }
      }
    }
    else {
      for (uint8_t index = 0; index < 2; ++index) {
        rgb_matrix_set_color(keys[index], beat_sin, 0, 0);
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

          // -SECTION START- LAYER INDICATOR FOR CONFIGURED KEYS IN WIN_FN
          for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
              for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                  uint8_t index = g_led_config.matrix_co[row][col];
                  uint16_t keycode = keymap_key_to_keycode(layer, (keypos_t){col,row});
                  if (index >= led_min && index <= led_max && index != NO_LED &&
                      keycode > KC_TRNS) {
                      switch(keycode) {
                        case RESET:
                          break;
                        case NK_TOGG:
                          break;
                        case GM_MODE:
                          break;
                        case M_SHUT:
                          break;
                        default:
                          rgb_matrix_set_color(index, RGB_GREEN);
                          break;
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
          if (gui_keys_enabled) { // Game Mode Indicator
            rgb_matrix_set_color(2, beat_sin, 0, 0); // DISABLED COLOR: RED
          }
          else {
            rgb_matrix_set_color(2, 0, beat_sin, 0); // ENABLED COLOR: BLUE
          }
          rgb_matrix_set_color(59, beat_sin, 0, 0);


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
#ifdef RAW_ENABLE
void raw_hid_receive(uint8_t *data, uint8_t length) {
    switch (data[0]) {
        case HID_SET_COLOR:
        {
            uint8_t h = data[1];
            uint8_t s = data[2];
            uint8_t v = data[3];
            rgb_matrix_sethsv_noeeprom(h,s,v);
            break;
        }
        case HID_SET_SPEED:
        {
            uint8_t speed = data[1];
            rgb_matrix_set_speed_noeeprom(speed);
            break;
        }
        case HID_SET_MODE:
        {
            uint8_t mode = data[1];
            rgb_matrix_mode_noeeprom(mode);
            break;
        }
        case HID_GET_AVAILABLE_MODES:
        {
            // THIS CODE IS S**T I KNOW
            // I HATE DEALING WITH ARRAYS IN C
            const uint8_t size = sizeof(rgb_matrix_effects_indexes) / sizeof(rgb_matrix_effects_indexes[0]);
            const uint8_t split_num = 1 + (size / length);
            const uint8_t chunk_size = size / split_num;
            uint8_t bonus = size - chunk_size * 2;
            uint8_t index = 0;
            uint8_t selected_num = data[1];
            if (selected_num == 0) {
                // IF SECOND BYTE IS 0 DO THIS
                uint8_t info[1] = {(1 + (size / length))};
                raw_hid_send(info, length);
                break;
            }

            for (uint8_t start = 0, end = chunk_size;
             start < size;
             start = end, end = start + chunk_size) {
                index++;
                if (bonus) {
                    end++;
                    bonus--;
                }

                if (index < selected_num) {
                    continue;
                }
                else if(index > selected_num) {
                    break;
                }
                uint8_t buffer[32] = {0}; // RAW_EPSIZE = 32
                for (uint8_t i = start; i < end; i++) {
                    uint8_t loop_length = (end - start);
                    uint8_t array_index = (i / loop_length);
                    uint8_t data_index = i - (array_index * loop_length);
                    buffer[data_index] = rgb_matrix_effects_indexes[i];
                }
                raw_hid_send(buffer, length);
            }
            break;
        }
        case HID_GET_CURRENT_RGB_SETTINGS:
        {
            HSV currenthsv = rgb_matrix_get_hsv();
            uint8_t currentmode = rgb_matrix_get_mode();
            uint8_t currentspeed = rgb_matrix_get_speed();
            // COLORS
            data[1] = currenthsv.h;
            data[2] = currenthsv.s;
            data[3] = currenthsv.v;
            // MODE
            data[4] = currentmode;
            // SPEED
            data[5] = currentspeed;

            // SENDING
            raw_hid_send(data, length);
            break;
        }
        case HID_SAVE_RGB_SETTINGS:
        {
            HSV currenthsv = rgb_matrix_get_hsv();
            uint8_t currentmode = rgb_matrix_get_mode();
            uint8_t currentspeed = rgb_matrix_get_speed();
            disable_rgb_tracked(false);
            // SAVE TO EEPROM
            rgb_matrix_mode(currentmode);
            rgb_matrix_set_speed(currentspeed);
            rgb_matrix_sethsv(currenthsv.h,currenthsv.s,currenthsv.v);
            break;
        }
        case HID_VOLUME:
        {
            SystemVolume = data[1];
            VolumeLevelIndicator(true);
            break;
        }
        default:
            raw_hid_send(data, length);
            break;
  }
}
#endif

