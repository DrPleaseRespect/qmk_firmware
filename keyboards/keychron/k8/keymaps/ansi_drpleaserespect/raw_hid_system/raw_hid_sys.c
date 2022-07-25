/* Copyright 2020 Kasper
 * Copyright 2022 DrPleaseRespect <DrPleaseRespect@outlook.com>
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
#include "raw_hid.h"
#include "raw_hid_sys.h"

const uint8_t rgb_matrix_effects_indexes[] = {
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

__attribute__ ((weak))
void disable_rgb_tracked(bool status) {
  if (status) {
    rgb_matrix_disable_noeeprom();
  }
  else {
    rgb_matrix_enable_noeeprom();
  }
}

void VolumeLevelIndicator(bool pressed){
    static uint16_t Volume_Timer = 0;
    uint8_t rgb_value;
    uint8_t id;
    if (pressed == true){
        Volume_Timer = timer_read();
    }
    if (timer_elapsed(Volume_Timer) < 1000) {
        uint8_t normalized_volume = (uint16_t)(SystemVolume * 10) / 100;
        rgb_value = ((SystemVolume % 10) * 255) / (10 - 1);
        if (normalized_volume == 0) {
            id = 0 + VolumeSTART; // ADD OFFSET
            rgb_matrix_set_color(id, 0, 0, rgb_value);

        }
        for (uint8_t index = 0; index < normalized_volume ; ++index) {
            id = index + VolumeSTART;
            if (index >= 9) {
                rgb_value = 255;
            }
            rgb_matrix_set_color(id, (255 - rgb_value), rgb_value, 0);

       }
    }
}



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
        // THIS RETURNS A RESULT TO THE HOST SYSTEM
        case HID_GET_AVAILABLE_MODES:
        {
            // THIS CODE IS S**T I KNOW
            // I HATE DEALING WITH ARRAYS IN C
            const uint8_t size = sizeof(rgb_matrix_effects_indexes) / sizeof(rgb_matrix_effects_indexes[0]);
            const uint8_t split_num = 1 + (size / (RAW_EPSIZE - 2));
            const uint8_t chunk_size = size / split_num;
            uint8_t bonus = size - chunk_size * 2;
            uint8_t index = 0;
            uint8_t selected_num = data[1];
            if (selected_num == 0) {
                // IF SECOND BYTE IS 0 DO THIS
                uint8_t info[3];
                info[0] = HID_GET_AVAILABLE_MODES;
                info[1] = selected_num;
                info[2] = split_num;
                raw_hid_send(info, RAW_EPSIZE);
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
                uint8_t buffer[RAW_EPSIZE] = {0};
                for (uint8_t i = start; i < end; i++) {
                    uint8_t loop_length = (end - start);
                    uint8_t array_index = (i / loop_length);
                    uint8_t data_index = i - (array_index * loop_length) + 2;
                    buffer[data_index] = rgb_matrix_effects_indexes[i];
                }
                buffer[0] = HID_GET_AVAILABLE_MODES;
                buffer[1] = index;
                raw_hid_send(buffer, RAW_EPSIZE);
            }
            break;
        }
        // THIS RETURNS A RESULT TO THE HOST SYSTEM
        case HID_GET_CURRENT_RGB_SETTINGS:
        {
            HSV currenthsv = rgb_matrix_get_hsv();
            uint8_t currentmode = rgb_matrix_get_mode();
            uint8_t currentspeed = rgb_matrix_get_speed();
            // COMMAND FOR VERIFICATION
            data[0] = HID_GET_CURRENT_RGB_SETTINGS;
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
