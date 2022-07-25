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

#pragma once

#define VolumeSTART 17
#define RAW_EPSIZE 32

enum raw_hid_functions {
  HID_SET_COLOR = 1,
  HID_SET_SPEED,
  HID_SET_MODE,
  HID_GET_AVAILABLE_MODES,
  HID_GET_CURRENT_RGB_SETTINGS,
  HID_SAVE_RGB_SETTINGS,
  HID_VOLUME,
};


// CODE BREAKS IF RAW_EPSIZE IS TOO SMALL
// (might increase in the future)
#if RAW_EPSIZE < 32
#   error RAW_EPSIZE TOO SMALL! (>=32)
#endif
uint8_t SystemVolume;
void VolumeLevelIndicator(bool pressed);
void disable_rgb_tracked(bool status);
