// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), 
            ENCODER_CCW_CW(KC_MPRV, KC_MNXT),
            ENCODER_CCW_CW(KC_BRID, KC_BRIU),  
        }
};
#endif

uint16_t oled_write_offset = 0;
bool oled_in_use = false;
uint32_t oled_raw_hid_timer = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┬───┬───┬───┐
     * │ 7 │ 8 │ 9 │ / │ / │ / │
     * ├───┼───┼───┼───┼───┼───┤
     * │ 4 │ 5 │ 6 │ * │ * │ * │
     * ├───┼───┼───┼───┼───┼───┤
     * │ 1 │ 2 │ 3 │ - │ - │ - │
     * └───┴───┴───┴───┴───┴───┘
     */
    [0] = LAYOUT(
        KC_P7,   KC_P8,   KC_P9,   KC_PSLS, KC_PSCR, QK_BOOT,
        KC_P4,   KC_P5,   KC_P6,   KC_PAST, KC_SCRL, KC_MPLY,
        KC_P1,   KC_P2,   KC_P3,   KC_PMNS, KC_PAUSE, DB_TOGG
    )
};

#ifdef OLED_ENABLE

static void render_logo(void) {
    static const char PROGMEM qmk_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
        0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00
    };

    oled_write_P(qmk_logo, false);
}

void create_volume(uint8_t volume) {
    uint8_t raw_logo[] = {
        0,254,254,254,  0,  0,254,254,254,  0,254,254,  6,  6,254,254,  0,254,254,  0,  0,  0,  0,254,254,  0,  0,254,254,  0,254,254,  6,252,224,252,  6,254,254,  0,254,254,134,134,134,  0, 48, 48,  0,254,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,254,  0,
        0,  0, 31, 31, 96, 96, 31, 31,  0,  0,127,127, 96, 96,127,127,  0,127,127, 96, 96, 96,  0,127,127, 96, 96,127,127,  0,127,127,  0,  0,  1,  0,  0,127,127,  0,127,127, 97, 97, 97,  0, 12, 12,  0,127, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,127,  0,
    };

    uint8_t inital_idx = 53;
    uint8_t next_row_offset = (16*8);
    uint16_t end_idx = (inital_idx+(((float)volume / 255)*70) );
    for (int i = inital_idx; i < end_idx; ++i)
    {
        raw_logo[i] |= 0b11110000;
        raw_logo[i+next_row_offset] |= 0b00001111;
    }
    oled_write_raw((const char*)raw_logo, sizeof(raw_logo));

}

void oled_render_boot(bool bootloader) {
    oled_clear();
    for (int i = 0; i < 16; i++) {
        oled_set_cursor(0, i);
        if (bootloader) {
            oled_write_P(PSTR("Awaiting New Firmware"), false);
        } else {
            oled_write_P(PSTR("Rebooting "), false);
        }
    }

    oled_render_dirty(true);
}

bool shutdown_user(bool jump_to_bootloader) {
    oled_render_boot(jump_to_bootloader);
    return false;
}

void user_msg_handler(uint8_t *data, const raw_packet *packet, uint8_t length) {
    switch (packet->subsystem) {
        case 100:
            oled_write_offset = 0;
            oled_set_cursor(0,0);
            break;

        case 101:
            oled_in_use = true;
            static uint8_t frame_buffer[1024];
            if (oled_write_offset + length <= 1024) {
                memcpy(&frame_buffer[oled_write_offset], data, length);
                oled_write_offset += length;
            }
            if (oled_write_offset >= 1024) {
                oled_write_raw((const char*)frame_buffer, 1024);
                oled_render_dirty(true);
                oled_write_offset = 0; // Reset for next frame
            }
            send_message((const uint8_t*)"", 1, 101);
            oled_raw_hid_timer = timer_read32() + 10000;
            break;
        case 102:
            oled_in_use = false;
            oled_set_cursor(0,0);
            break;
    }
}

bool oled_task_user(void) {
  if (oled_in_use && !timer_expired32(timer_read32(), oled_raw_hid_timer)) {
    return false;
  }
  render_logo();
  return true;
}

#endif