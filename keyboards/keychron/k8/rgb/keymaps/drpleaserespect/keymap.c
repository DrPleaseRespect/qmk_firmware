/* Copyright 2020 Adam Honse <calcprogrammer1@gmail.com>
 * Copyright 2020 Dimitris Mantzouranis <d3xter93@gmail.com>
 * Copyright 2022 Harrison Chan (Xelus)
 * Copyright 2022 Thomas Bowman Mørch (tbowmo)
 * Copyright 2023 DrPleaseRespect <DrPleaseRespect@outlook.com>
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
#include "./features/password_sys/pass_sys.h"
#include "eeprom_definitions.h"



#ifdef BLUETOOTH_ENABLE
#include "iton_bt.h"
#include "outputselect.h"
#endif

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layer_names {
    WIN_BASE    = 0,
    SPACE_CA       ,
    WIN_FN         ,
    MACRO          ,
};

enum custom_keycodes {
  GM_MODE = SAFE_RANGE,
  S_CADET,
  P_LOCK,
  M_SHUT,
  KC_BT1,
  KC_BT2,
  KC_BT3,
  KC_USB,
  KC_PAIR,
};


static bool init_eeprom = false; // Only Sets to True if EEPROM has been Reset
static bool rgb_enabled = true;
static bool gui_keys_enabled = true;
static bool shutdown_macro_held = false;
static bool shutdown_initiated = false;


static uint16_t blink_timer;


#define KC_TASK LGUI(KC_TAB)        // Task viewer
#define KC_FLXP LGUI(KC_E)          // Windows file explorer
#define KC_CRTN LGUI(KC_C)          // Cortana | Microsoft Teams

#define KC_MSSN LGUI(KC_F3)         // Mission Control
#define KC_FIND LALT(LGUI(KC_SPC))  // Finder
#define KC_SIRI LGUI(KC_SPC)        // Siri
#define KC_MSNP LSFT(LGUI(KC_4))    // Mac snip tool

#define MVL_FN  MO(WIN_FN)
#define MV_MACR MO(MACRO)

#define SPACE_CA_STATE IS_LAYER_ON(SPACE_CA)
#define ADDON_START    SPACE_CA
#define ADDON_END      SPACE_CA
#define FUNC_START     WIN_FN
#define FUNC_END       MACRO


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
  // BASE LAYERS
  [WIN_BASE] = LAYOUT_tkl_ansi(
      KC_ESC,           KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,    KC_F12,      KC_PSCR,  KC_VOLD,  KC_VOLU,
      KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,    KC_BSPC,     KC_INS,   KC_HOME,  KC_PGUP,
      KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,   KC_BSLS,     KC_DEL,   KC_END,   KC_PGDN,
      KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,            KC_ENT,
      KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT,                          KC_UP,
      KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                                      KC_RALT, KC_RGUI, MVL_FN,    KC_RCTL,     KC_LEFT,  KC_DOWN,  KC_RGHT
  ),
  // ADD-ON LAYERS - ADDONS TO THE BASE LAYER
  // SPACE CADET LAYER - CHANGE SHIFT KEYS TO SPACE CADET KEYS
  [SPACE_CA] = LAYOUT_tkl_ansi(
          _______,         _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   _______,     _______,  _______,  _______,
        _______, _______, _______ , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   _______,     _______,  _______,  _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______,     _______,  _______,  _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ , _______, _______,            _______,
        KC_LSPO, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          KC_RSPC,                         _______,
        _______, _______, _______,                   _______,                                     _______, _______, _______,   _______,     _______, _______,  _______
    ),
  // FUNCTION LAYERS - FN LAYER
  [WIN_FN] = LAYOUT_tkl_ansi(
        QK_BOOT,          NK_TOGG, GM_MODE, S_CADET, _______, _______, _______, KC_BRID, KC_BRIU, _______, _______, _______,   KC_MSTP,     KC_MPLY,  KC_MPRV,  KC_MNXT,
        _______, _______, KC_NUM , KC_PSLS, KC_PAST, _______, _______, _______, _______, KC_BT1 , KC_BT2 , KC_BT3 , KC_USB ,   _______,     RGB_SPI,  RGB_SAI,  RGB_HUI,
        _______, KC_KP_7, KC_KP_8, KC_KP_9, KC_PMNS, _______, _______, _______, _______, _______, M_SHUT , _______, _______,   RGB_TOG,     RGB_SPD,  RGB_SAD,  RGB_HUD,
        _______, KC_KP_4, KC_KP_5, KC_KP_6, KC_PPLS, _______, _______, _______, _______, P_LOCK , _______, _______,            _______,
        _______, KC_KP_1, KC_KP_2, KC_KP_3, KC_PENT, _______, _______, _______, _______, _______, _______,          MV_MACR,                          RGB_VAI,
        _______, KC_PDOT, KC_KP_0,                   _______,                                     _______, _______, XXXXXXX,   _______,     RGB_RMOD, RGB_VAD,  RGB_MOD
    ),
  [MACRO] = LAYOUT_tkl_ansi(
         XXXXXXX,          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX,  XXXXXXX,  XXXXXXX,
         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX,  XXXXXXX,  XXXXXXX,
         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX,  XXXXXXX,  XXXXXXX,
         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,
         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,                         XXXXXXX,
         XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX,                                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX,  XXXXXXX,  XXXXXXX
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

#ifdef BLUETOOTH_ENABLE
#ifndef DEFERRED_EXEC_ENABLE
#error "DEFERRED_EXEC_ENABLE needs to be enabled for bluetooth"
#endif


enum profile_states_t {
    STATE_CONNECTING,
    STATE_CONNECTED,
    STATE_PAIRING,
    STATE_DISCONNECTED,
};

enum profiles_t {
    PROFILE_BT_1 = 0,
    PROFILE_BT_2 = 1,
    PROFILE_BT_3 = 2,
    PROFILE_USB = 3,
};

#define PAIRING_LONG_PRESS  3000
#define DELAY_SLOW_BLINKING 100
#define DELAY_FAST_BLINKING 50
#define BLUETOOTH_LED_OFFSET 25

static bool bluetooth_enabled = false;
static bool btprofile_led = false;
static uint8_t current_profile_state = STATE_DISCONNECTED;
static deferred_token deferred_bt_pairing = INVALID_DEFERRED_TOKEN;
static uint16_t bt_keypress_time;

void bluetooth_eeprom_reset(void) {
    eeprom_config.raw = eeconfig_read_user();
    eeprom_config.bt_profile = PROFILE_USB;
}

/**
 * Deferred execution for entering pairing mode
*/
uint32_t start_pairing(uint32_t trigger_time, void *cb_arg) {
    /* Initiate pairing mode */
    iton_bt_enter_pairing();
    return 0;
}

/**
 * Deferred execution for LED blinking
*/
uint32_t bt_led_blinking(uint32_t trigger_time, void *cb_arg) {
    btprofile_led = !btprofile_led;

    switch (current_profile_state) {
        case STATE_PAIRING:
            return DELAY_SLOW_BLINKING;
        case STATE_DISCONNECTED:
            if (bt_keypress_time == 0 || timer_elapsed32(bt_keypress_time) > 3000) {
                bt_keypress_time = 0;
                btprofile_led = false;
            }
            return DELAY_FAST_BLINKING;
        case STATE_CONNECTING:
            // current_profile_state = STATE_CONNECTED;
            // static led on during bootup in bt mode for 1 second
            btprofile_led = true;
            set_output(OUTPUT_BLUETOOTH);
            iton_bt_switch_profile(eeprom_config.bt_profile);
            return 1000;
        case STATE_CONNECTED:
        default:
            btprofile_led = false;
            return 1000;
    }
}

/**
 * Internal profile selection, storing the selected profile
 * in eeprom, so it can be retrieved on bootup for flashing correct
 * indicators
*/
void select_profile(uint8_t profile) {
    dprintf("profile %u, enabled %u", profile, bluetooth_enabled);
    if (bluetooth_enabled) {
        if (profile < PROFILE_USB) {
            set_output(OUTPUT_BLUETOOTH);
            // Start pairing in 2 seconds (Will be cancelled on key release)
            if (!extend_deferred_exec(deferred_bt_pairing, PAIRING_LONG_PRESS)) {
                deferred_bt_pairing = defer_exec(PAIRING_LONG_PRESS, start_pairing, NULL);
            }
            iton_bt_switch_profile(profile);
        } else {
            set_output(OUTPUT_USB);
            eeprom_config.bt_profile = PROFILE_USB;
        }
        current_profile_state = STATE_CONNECTING;
        if (eeprom_config.bt_profile == profile) {
            current_profile_state = STATE_CONNECTED;
            return;
        }
        if (eeprom_config.bt_profile != profile) {
            eeprom_config.bt_profile = profile;
            eeconfig_update_user(eeprom_config.raw);
        }


    }
}

/**
 * ITON callback methods, setting a local state variable
*/
void iton_bt_connection_successful() {
    current_profile_state = STATE_CONNECTED;
    set_output(OUTPUT_BLUETOOTH);
}

void iton_bt_entered_pairing() {
    current_profile_state = STATE_PAIRING;
};

void iton_bt_disconnected() {
    bt_keypress_time = timer_read();
    current_profile_state = STATE_DISCONNECTED;
};

void iton_bt_enters_connection_state() {
    current_profile_state = STATE_CONNECTING;
};


/**
 * Setting led on / off (blinking freq defined in deferred exec blink routine)
*/

bool bluetooth_led(void) {
    if (btprofile_led) {
        rgb_matrix_set_color_all(0,0,0);
        rgb_matrix_set_color(BLUETOOTH_LED_OFFSET + eeprom_config.bt_profile, 0, 0, 255);
    }

    return true;
}

bool bluetooth_key_processing(uint16_t keycode, keyrecord_t *record) {
    if (bluetooth_enabled) {
        if (record->event.pressed) {
            switch (keycode) {
                case KC_BT1:
                    select_profile(PROFILE_BT_1);
                    return false;
                case KC_BT2:
                    select_profile(PROFILE_BT_2);
                    return false;
                case KC_BT3:
                    select_profile(PROFILE_BT_3);
                    return false;
                case KC_USB:
                    select_profile(PROFILE_USB);
                    return false;
                case KC_PAIR:
                    iton_bt_enter_pairing();
                    return false;
            }
        } else if (keycode >= KC_BT1 && keycode <= KC_BT3) {
            cancel_deferred_exec(deferred_bt_pairing);
            return false;
        }
    }
    return true;
}

bool dipswitch_bluetooth(uint8_t index, bool active) {
    switch (index) {
        case 0:
            return false;
        case 1:
            if (active) {
                bluetooth_enabled = false;
                select_profile(PROFILE_USB);
            } else {
                bluetooth_enabled = true;
                if (eeprom_config.bt_profile == PROFILE_USB) {
                    set_output(OUTPUT_USB);
                } else {
                    set_output(OUTPUT_NONE);
                }
                current_profile_state = STATE_CONNECTING;
                defer_exec(100, bt_led_blinking, NULL);
            }
            return false;
    }
    return true;
}

#endif

// get_millisecond_timer function for lib8tion
uint32_t get_millisecond_timer(void) {
  return timer_read32();
}


uint16_t get_trans_key_origin(keypos_t keyposition){
  uint8_t transk_origin_layer = layer_switch_get_layer(keyposition);
  uint16_t transk_origin_keycode = keymap_key_to_keycode(
    transk_origin_layer, keyposition
  );
  return transk_origin_keycode;
}

#ifdef DEFERRED_EXEC_ENABLE
void shutdown_indicator(void) {
  if (shutdown_macro_held) {
    uint8_t beat = beat8(255, 0); // Sawtooth Wave
    // Sawtooth to Square Wave
    if ((beat % 4) > 1) {
      beat = 255;
    }
    else {
      beat = 0;
    }
    if (!shutdown_initiated) {
      rgb_matrix_set_color_all(beat, 0, 0);
    }
    else {
      rgb_matrix_set_color_all(0, random8(), beat);
    }
  }
}

uint32_t shutdown_loop(uint32_t trigger_time, void *cb_arg) {
    SEND_STRING(SS_LGUI("r"));
    wait_ms(500);
    SEND_STRING("shutdown -s -t 0 -f");
    wait_ms(500);
    SEND_STRING(SS_TAP(X_ENTER));
    return shutdown_macro_held ? 3000 : 0;
}

uint32_t shutdown_callback(uint32_t trigger_time, void *cb_arg) {
  static deferred_token shutdown_loop_token = INVALID_DEFERRED_TOKEN;
  if (shutdown_loop_token != INVALID_DEFERRED_TOKEN) {
    cancel_deferred_exec(shutdown_loop_token);
    shutdown_loop_token = INVALID_DEFERRED_TOKEN;
  }
  shutdown_initiated = true;
  if (shutdown_loop_token == INVALID_DEFERRED_TOKEN) {
    shutdown_loop_token = defer_exec(500, shutdown_loop, NULL);
  }
  return 0;
}

#endif

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

bool dip_switch_update_user(uint8_t index, bool active){
#ifdef BLUETOOTH_ENABLE
  dipswitch_bluetooth(index, active);
#endif
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
  // PASS SYSTEM DISPLAY HOOK
  display_pass_index();

#ifdef DEFERRED_EXEC_ENABLE
  // SHUTDOWN BLINK HOOK
  shutdown_indicator();
#endif

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

  if (!gui_keys_enabled || SPACE_CA_STATE) {
    // I can make this not use keyboard specific coordinates but for performance's sake i'll do it this way..
    uint8_t keys[4] = {77, 81, 63, 74}; // Specific Coordinates for K8
    uint8_t rows[4] = {5,5, 4, 4};    // Specific Coordinates for K8
    uint8_t col[4] = {1, 11, 0, 13};   // Specific Coordinates for K8
    uint8_t layer = get_highest_layer(layer_state);
    if (layer >= FUNC_START) {

      for (uint8_t index = 0; index < 4; ++index) {

        uint16_t keycode = get_trans_key_origin((keypos_t){col[index],rows[index]});
        if (keycode == KC_LGUI || keycode == KC_RGUI) {
          // GAME MODE
          if (!gui_keys_enabled && index < 2) {
            rgb_matrix_set_color(keys[index], beat_sin, 0, 0);
          }
        }
        if (keycode == KC_LSPO || keycode == KC_RSPC) {
          // SPACE CADET
          if (SPACE_CA_STATE && index > 1) {
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
          if (SPACE_CA_STATE && index > 1){
            rgb_matrix_set_color(keys[index], beat_sin, 0, 0);
          }
      }
    }
  }

  // -SECTION END-
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    bool caps_override = false;
    uint8_t layer = get_highest_layer(layer_state);
    const uint16_t blocked_keys[] = {
      QK_BOOT, NK_TOGG, GM_MODE, M_SHUT, S_CADET, P_LOCK,  KC_BT1, KC_BT2, KC_BT3, KC_USB, KC_PAIR,
    };
    const size_t blocked_keys_size = sizeof(blocked_keys) / sizeof(blocked_keys[0]);
    // -SECTION START- LAYER INDICATOR
    if (layer > SPACE_CA) {
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
                      break; // BREAK OUT ONCE A BLOCKED KEY IS FOUND
                    }
                  }
                  if (!blocked_key) {
                    switch (layer) {
                      case WIN_FN:
                        rgb_matrix_set_color(index, RGB_GREEN);
                        break;
                      case MACRO:
                        rgb_matrix_set_color(index, RGB_RED);
                        break;
                      default:
                        break;
                    }
                }
              }
            }
        }
    }
    // -SECTION END-

    // -SECTION START- PER-LAYER RGB STUFF
    switch (layer) {
        case WIN_FN:
          caps_override = true;


          // -SECTION START-  NUMLOCK INDICATOR
          if (host_keyboard_led_state().num_lock) {
            rgb_matrix_set_color(18, RGB_GREEN);
          }
          else {
            rgb_matrix_set_color(18, RGB_RED);
          }
          // -SECTION END-

          // -SECTION START-  BLINKING RGB LIGHTS

          // RESET LED
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

          // M_SHUT MACRO
          rgb_matrix_set_color(43, beat_sin, 0, 0);

          // PASS LOCK BUTTON
          rgb_matrix_set_color(59, beat_sin, 0, 0);

          // SPACE CADET KEY
          if (SPACE_CA_STATE) {
            rgb_matrix_set_color(3, 0, beat_sin, 0); // ENABLED COLOR: GREEN
          }
          else {
            rgb_matrix_set_color(3, beat_sin, 0, 0); // DISABLED COLOR: RED
          }


          // -SECTION END-
          break;

        case MACRO:
          caps_override = true;
          break;

        default:
          break;
    }
    // -SECTION END-

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

#ifdef BLUETOOTH_ENABLE
    // -SECTION START- BLUETOOTH INDICATOR
    bluetooth_led();
    // -SECTION END-
#endif

    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // PASS SYSTEM KEY HOO
  bool pass_sys_hook = pass_hook(record);
  if (!pass_sys_hook) {
    return false;
  }

  // BLUETOOTH SYSTEM
#ifdef BLUETOOTH_ENABLE
  bool bluetooth_process = bluetooth_key_processing(keycode, record);
  if (!bluetooth_process) {
    return false;
  }
#endif

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
#ifdef DEFERRED_EXEC_ENABLE
    case M_SHUT:
      {
          static deferred_token shutdown_token = INVALID_DEFERRED_TOKEN;
          if (record->event.pressed) {
            if (shutdown_token == INVALID_DEFERRED_TOKEN) {
              shutdown_token = defer_exec(1000, shutdown_callback, NULL);
            }
            shutdown_macro_held = true;

          }
          else {
            cancel_deferred_exec(shutdown_token);
            shutdown_token = INVALID_DEFERRED_TOKEN;
            shutdown_initiated = false;
            shutdown_macro_held = false;

          }
          return true;
      }
#endif
    // SPACE CADET SYSTEM
    case S_CADET:
      if (record->event.pressed) {
        if (SPACE_CA_STATE) {
          layer_off(SPACE_CA);
        }
        else {
          layer_on(SPACE_CA);
        }
      }
      return true;

    // PASS SYSTEM KEY
    case P_LOCK:
        lock_pass();
        return true;
    default:
      return true;
  }
}



void matrix_scan_user(void) {
  //matrix_status_indicators();

}


void keyboard_pre_init_user(void) {
  blink_timer = timer_read();
}

void eeconfig_init_user(void) {
  init_eeprom = true; // Enabled due to reset of EEPROM
  bluetooth_eeprom_reset();
  pass_sys_eeconfig_init_hook();
}

void keyboard_post_init_user(void) {
#ifdef BLUETOOTH_ENABLE
    eeprom_config.raw = eeconfig_read_user();
    if (eeprom_config.bt_profile >= PROFILE_USB) {
        eeprom_config.bt_profile = 0;
    }
#endif
  // Customise these values to desired behaviour
  //debug_enable=true;
  //debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
  pass_sys_kb_postinit_hook();
  if (init_eeprom) { // Only Execute if eeprom is reset
    rgb_matrix_enable();
    rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv(180, 255, 255);
  }
}
