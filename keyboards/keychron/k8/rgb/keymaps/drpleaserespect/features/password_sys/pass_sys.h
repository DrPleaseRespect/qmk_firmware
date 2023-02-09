/* Copyright 2022 DrPleaseRespect <DrPleaseRespect@outlook.com>
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

#include <quantum.h>
#include "lib/lib8tion/lib8tion.h"

uint32_t get_millisecond_timer(void);
bool pass_hook(keyrecord_t *record);
void display_pass_index(void);
void lock_pass(void);
bool pass_sys_isunlocked(void);
bool pass_sys_islocked(void);
void pass_sys_kb_postinit_hook(void);
void pass_sys_eeconfig_init_hook(void);
void eeconfig_init_user(void);
void keyboard_post_init_user(void);
