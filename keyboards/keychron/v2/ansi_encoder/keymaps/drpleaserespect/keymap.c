/* Copyright 2021 @ Keychron (https://www.keychron.com)
 * Copyright 2023-2024 DrPleaseRespect <DrPleaseRespect@outlook.com>
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
//#include "keychron_common.h"
#include "lib/lib8tion/lib8tion.h"
#include "./features/gamemode/gamemode.h"
#include "./features/encoder_spinner/encoder_spinner.h"


// clang-format off

// Custom Keycodes
enum custom_keycodes {
    GM_MODE = SAFE_RANGE,
    CW_Key,
    CCW_Key,
    ButtonKey,
};

// Layers
enum layers {
    WIN_BASE,
    WIN_FN,
    _FN3,
    _FN_KEYS,
};


void rgb_matrix_toggle_override(void) {
        switch (rgb_matrix_get_flags()) {
        case LED_FLAG_ALL: {
            rgb_matrix_set_flags(LED_FLAG_NONE);
            rgb_matrix_set_color_all(0, 0, 0);
        } break;
        default: {
            rgb_matrix_set_flags(LED_FLAG_ALL);
        } break;
    }
    if (!rgb_matrix_is_enabled()) {
        rgb_matrix_set_flags(LED_FLAG_ALL);
        rgb_matrix_enable();
    }
    }

// encoder_spinner.h thing
#define SPINNER_SIZE 5
void process_encoder_keys(uint8_t pos, uint8_t flags) {
    switch (pos) {
        case 0:
            if (flags & SPINNER_CCW) {
                rgb_matrix_step_reverse();
            } else if (flags & SPINNER_CW) {
                rgb_matrix_step();
            } else if (flags & SPINNER_BUTTON) {
                rgb_matrix_toggle_override();
            }
            break;
        case 1:
            if (flags & SPINNER_CCW) {
                rgb_matrix_decrease_val();
            } else if (flags & SPINNER_CW) {
                rgb_matrix_increase_val();
            } else if (flags & SPINNER_BUTTON) {
                rgb_matrix_toggle_override();
            }
            break;
        case 2:
            if (flags & SPINNER_CCW) {
                rgb_matrix_decrease_hue();
            } else if (flags & SPINNER_CW) {
                rgb_matrix_increase_hue();
            } else if (flags & SPINNER_BUTTON) {
                rgb_matrix_toggle_override();
            }
            break;
        case 3:
            if (flags & SPINNER_CCW) {
                rgb_matrix_decrease_sat();
            } else if (flags & SPINNER_CW) {
                rgb_matrix_increase_sat();
            } else if (flags & SPINNER_BUTTON) {
                rgb_matrix_toggle_override();
            }
            break;
        case 4:
            if (flags & SPINNER_CCW) {
                rgb_matrix_decrease_speed();
            } else if (flags & SPINNER_CW) {
                rgb_matrix_increase_speed();
            } else if (flags & SPINNER_BUTTON) {
                rgb_matrix_toggle_override();
            }
            break;
    }
}




// Retro Tapping
#define ALT_FN LM(_FN_KEYS,  MOD_LALT)
#define CTRL_FN LM(_FN_KEYS, MOD_LCTL)

bool get_retro_tapping(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case ALT_FN:
        case CTRL_FN:
            return true;
        default:
            return false;
    }
};


// Key Indicators

// Unused Utility Function
//uint16_t get_trans_key_origin(keypos_t keyposition) {
//  uint8_t transk_origin_layer = layer_switch_get_layer(keyposition);
//  uint16_t transk_origin_keycode = keymap_key_to_keycode(
//    transk_origin_layer, keyposition
//  );
//  return transk_origin_keycode;
//};


bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t layer = get_highest_layer(layer_state);
    if (layer < WIN_FN) {
        switch (rgb_matrix_get_flags()) {
            case LED_FLAG_NONE:
                rgb_matrix_set_color_all(RGB_BLACK);
                break;
            default:
                break;
        }
    }
    // Iterate through ROWS and COLS
    for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
        for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
            uint8_t index = g_led_config.matrix_co[row][col];

            uint8_t key_origin_layer = layer_switch_get_layer((keypos_t){col,row});
            uint16_t keycode = keymap_key_to_keycode(key_origin_layer, (keypos_t){col,row});

            // RGB only on configured keycodes
            if (get_highest_layer(layer_state) > 0) {
                if (index >= led_min && index < led_max && index != NO_LED &&
                    !(key_origin_layer > WIN_BASE)) {
                    rgb_matrix_set_color(index, RGB_BLACK);
                }
            }

            // Keycode Toggle Indicator
            switch (keycode) {
                case NK_TOGG:
                    if (keymap_config.nkro) {
                        rgb_matrix_set_color(index, 0, 255, 255);
                    } else {
                        rgb_matrix_set_color(index, 255, 0, 0);
                    }
                    break;
                case KC_NUM:
                    if (host_keyboard_led_state().num_lock) {
                        rgb_matrix_set_color(index, RGB_GREEN);
                    } else {
                        rgb_matrix_set_color(index, RGB_RED);
                    }
                    break;
                case AC_TOGG:
                    if (autocorrect_is_enabled()) {
                        rgb_matrix_set_color(index, RGB_GREEN);
                    } else {
                        rgb_matrix_set_color(index, RGB_RED);
                    }
                    break;
                case ButtonKey: // Only Show RGB Display if ButtonKey is in layer
                    spinner_rgb_indicator(); // Spinner RGB Hook
                default:
                    break;

            }
            // Game Mode Hook
            rgb_matrix_gm_hook(GM_MODE, keycode, index);
            
        }
    }
    return true;
};

// Combos
const uint16_t PROGMEM backtick_combo[] = {KC_LGUI, KC_LSFT, QK_GESC, COMBO_END};

combo_t key_combos[] = {
    COMBO(backtick_combo, KC_GRV)
};


// KEYCODE MACROS
#define WINTOGG MO(WIN_FN)
#define FNTOGG  MO(_FN_KEYS)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [WIN_BASE] = LAYOUT_ansi_67(
        QK_GESC,  KC_1,     KC_2,     KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,         KC_MPLY,
        KC_TAB,  KC_Q,     KC_W,     KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,          KC_HOME,
        KC_CAPS, KC_A,     KC_S,     KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,  KC_QUOT,            KC_ENT,           KC_DEL,
        KC_LSFT,           KC_Z,     KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,   KC_SLSH,            FNTOGG, KC_UP,
        CTRL_FN, KC_LGUI,  ALT_FN,                              KC_SPC,                             KC_RALT,  WINTOGG,  TT(_FN3), KC_LEFT, KC_DOWN, KC_RGHT),

    [WIN_FN] = LAYOUT_ansi_67(
        KC_GRV,  KC_F1,    KC_F2,    KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,   KC_F12,  AC_TOGG,           ButtonKey,
        RGB_TOG, RGB_MOD,  RGB_VAI,  RGB_HUI, RGB_SAI, RGB_SPI, _______, _______, _______, _______, _______,  _______,  _______,  _______,          KC_PGUP,
        _______, RGB_RMOD, RGB_VAD,  RGB_HUD, RGB_SAD, RGB_SPD, _______, _______, _______, _______, _______,  _______,            _______,          KC_PGDN,
        _______,           _______,  _______, _______, _______, _______, NK_TOGG, _______, _______, _______,  _______,            KC_RSFT, KC_PGUP,
        _______, GM_MODE,  _______,                             _______,                            _______,  _______,  _______,  _______, KC_PGDN, _______),

    [_FN3] = LAYOUT_ansi_67(
        KC_TILD, KC_F1,    KC_F2,    KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,   KC_F12,   _______,          KC_MUTE,
        KC_NUM,  KC_KP_7,  KC_KP_8,  KC_KP_9, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,  _______,          KC_INS,
        _______, KC_KP_4,  KC_KP_5,  KC_KP_6, _______, KC_BTN1, KC_BTN2, _______, _______, _______, _______,  _______,            _______,          KC_END,
        _______, KC_KP_1,  KC_KP_2,  KC_KP_3, _______, _______, _______, _______, _______, _______, _______,                      _______, KC_MS_U,
        _______, KC_PDOT,  KC_KP_0,                             _______,                            _______,  _______,  _______,  KC_MS_L, KC_MS_D, KC_MS_R),

    [_FN_KEYS] = LAYOUT_ansi_67(
        KC_ESC,  KC_F1,    KC_F2,    KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,   KC_F12,   _______,          QK_BOOT,
        _______, _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,  _______,          _______,
        _______, _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______,  _______,            _______,          _______,
        _______,           _______,  _______, _______, _______, _______, _______, _______, _______, _______,  _______,            _______, _______,
        _______, _______,  _______,                             _______,                            _______,  _______,  _______,  _______, _______, _______),
};

// clang-format on

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [WIN_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [WIN_FN]   = { ENCODER_CCW_CW(CCW_Key, CW_Key)},
    [_FN3]   = { ENCODER_CCW_CW(KC_MPRV, KC_MNXT)},
    [_FN_KEYS] = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX)}
};
#endif // ENCODER_MAP_ENABLE

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    //if (!process_record_keychron(keycode, record)) {
    //    return false;
    //}
    GM_HOOK(GM_MODE, keycode, record);
    spinner_key(CW_Key, CCW_Key, ButtonKey, keycode, record);
    return true;
};

void matrix_scan_user(void) {
    matrix_scan_spinner();
}

bool dip_switch_update_user(uint8_t index, bool active) {
    return false;
};

void keyboard_post_init_user(void) {
#ifdef CONSOLE_ENABLE
  // Customise these values to desired behaviour
  debug_enable=true;
  //debug_matrix=true;
  debug_keyboard=true;
  debug_mouse=true;
#endif
}