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

#pragma once

#define RGBSTATUS_DURATION 300 // in miliseconds

#define RGBSTATUS_DISPLAY_IDX 16


void display_status(uint8_t value, uint8_t max_value);

void display_rgb_mode(void);

void display_rgb_val(void);

void display_rgb_hue(void);

void display_rgb_sat(void);

void display_rgb_speed(void);

void stop_rgb_status(void);
