/* Copyright 2024-2025 DrPleaseRespect <DrPleaseRespect@outlook.com>
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

#define CONTROL_ENABLE

#ifndef RAW_ENABLE
#error "RAWHID (RAW_ENABLE) must be enabled!"
#endif


#define MAX_DATA_SIZE 255
#define PACKET_SIZE 32
#define PAYLOAD_SIZE 3
#define DATA_SIZE PACKET_SIZE - PAYLOAD_SIZE

#define CTRL_VERSION_MAJOR 1
#define CTRL_VERSION_MINOR 0

#include QMK_KEYBOARD_H




struct raw_packet {
    // SUBSYSTEM ENUM
    uint8_t subsystem;
    // PACKETID 0-255
    // If Multipart:  packet ID must be the same as last Multipart Packet
    // If Singlepart: packet ID is ignored
    uint8_t id;
    // LENGTH OF ENTIRE DATA (Length of Entire Multipart Data)
    // Must be the same throughout entire Multipart Message
    uint8_t length;
    // DATA
    uint8_t data[DATA_SIZE];
} __attribute__((packed));

typedef struct raw_packet raw_packet;


enum Subsystems {
    // ERROR STATE
    ERROR_STATE = 0,

    // RAW HID IMPLEMENTATION DATA
    GET_PROTOCOL_INFO = 1,

    // KEYBOARD SYSTEMS
    // -PC SIDE-
    // HANDLED BY HID BROKER
    #ifdef RGB_MATRIX_ENABLE
    GET_RGB_SETTINGS  = 2,
    GET_RGB_MODES     = 3,
    SET_RGB_SETTINGS  = 4,

    PER_RGB_CONTROLS  = 5,
    #endif

    VOLUME = 6,


    // BROADCAST MESSAGE
    BROADCAST = 99,
    
    // 100-255 Range
    SUBSYS_SAFERANGE = 100,
    // CUSTOM SYSTEMS
    // -PC SIDE-
    // HANDLED BY EXTERNAL PROGRAMS
    SUBSYS_MAXRANGE = 255,
};

void send_message(const uint8_t *data, uint8_t length, uint8_t subsystem);
void user_msg_handler(uint8_t *data, const raw_packet *packet, uint8_t length);
void raw_hid_receive(uint8_t *data, uint8_t length);
uint8_t get_volume(void);