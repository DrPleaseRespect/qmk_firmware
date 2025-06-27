/* Copyright 2020 Kasper
 * Copyright 2025 DrPleaseRespect <DrPleaseRespect@outlook.com>
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

const uint8_t enabled_rgb_matrix_effect_ids[] = {
#ifdef ENABLE_RGB_MATRIX_SOLID_COLOR
    1,
#endif
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
#ifdef ENABLE_RGB_MATRIX_CYCLE_OUT_IN
    15,
#endif
#ifdef ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
    16,
#endif
#ifdef ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
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
#ifdef ENABLE_RGB_MATRIX_FLOWER_BLOOMING
    23,
#endif
#ifdef ENABLE_RGB_MATRIX_RAINDROPS
    24,
#endif
#ifdef ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
    25,
#endif
#ifdef ENABLE_RGB_MATRIX_HUE_BREATHING
    26,
#endif
#ifdef ENABLE_RGB_MATRIX_HUE_PENDULUM
    27,
#endif
#ifdef ENABLE_RGB_MATRIX_HUE_WAVE
    28,
#endif
#ifdef ENABLE_RGB_MATRIX_PIXEL_FRACTAL
    29,
#endif
#ifdef ENABLE_RGB_MATRIX_PIXEL_FLOW
    30,
#endif
#ifdef ENABLE_RGB_MATRIX_PIXEL_RAIN
    31,
#endif
#ifdef ENABLE_RGB_MATRIX_TYPING_HEATMAP
    32,
#endif
#ifdef ENABLE_RGB_MATRIX_DIGITAL_RAIN
    33,
#endif
#ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
    34,
#endif
#ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE
    35,
#endif
#ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
    36,
#endif
#ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
    37,
#endif
#ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
    38,
#endif
#ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
    39,
#endif
#ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
    40,
#endif
#ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
    41,
#endif
#ifdef ENABLE_RGB_MATRIX_SPLASH
    42,
#endif
#ifdef ENABLE_RGB_MATRIX_MULTISPLASH
    43,
#endif
#ifdef ENABLE_RGB_MATRIX_SOLID_SPLASH
    44,
#endif
#ifdef ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
    45,
#endif
#ifdef ENABLE_RGB_MATRIX_STARLIGHT
    46,
#endif
#ifdef ENABLE_RGB_MATRIX_STARLIGHT_SMOOTH
    47,
#endif
#ifdef ENABLE_RGB_MATRIX_STARLIGHT_DUAL_HUE
    48,
#endif
#ifdef ENABLE_RGB_MATRIX_STARLIGHT_DUAL_SAT
    49,
#endif
#ifdef ENABLE_RGB_MATRIX_RIVERFLOW
    50,
#endif
};
