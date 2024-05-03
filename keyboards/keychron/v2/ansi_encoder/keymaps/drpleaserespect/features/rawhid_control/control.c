/* Copyright 2023 DrPleaseRespect <DrPleaseRespect@outlook.com>
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


#include QMK_KEYBOARD_H
#include "raw_hid.h"
#include "control.h"


typedef enum {
    // First bit is reserved for multipart Subsystems
    // 0-127   : range for non-multipart Subsystems
    // 128-255 : range of multipart Subsystems
	REQ_VERSION         = 0,
    SET_HSV_COLORS      = 1,
    GET_HSV_COLORS      = 2,
    BROADCAST           = 127,
    
} Subsystems;

typedef struct {
    uint8_t current_chunk;
    uint8_t total_chunk;
} MultipartInfo;

#define MULTIPART_BIT 0b10000000
#define SINGLEPART_OFFSET 1
#define MULTIPART_OFFSET 3
#define IS_MULTIPART(Subsystem) ((command & MULTIPART_BIT) != 0)
#define LEN(array) (sizeof(array) / sizeof(array[0]))

#define MULTIPARTINFO_CURRENTBITS 0b11110000
#define MULTIPARTINFO_TOTALBITS   0b00001111

#define HIDPROC_MAJOR    1
#define HIDPROC_MINOR    0
#define HIDPROC_REVISION 0

//void version_set(Subsystem subsystem, uint8_t *packet, uint8_t length)

MultipartInfo parse_multipartinfo(uint8_t multipartinfo_data) {
    MultipartInfo multipartdata;

    multipartdata.current_chunk = (multipartinfo_data & MULTIPARTINFO_CURRENTBITS) >> 4;
    multipartdata.total_chunk   = (multipartinfo_data & MULTIPARTINFO_TOTALBITS);
    return multipartdata;
}


uint8_t create_multipartinfo(uint8_t current_chunk, uint8_t total_chunk) {
    // 15 Chunk Max

    uint8_t _current_chunk = current_chunk << 4;
    uint8_t _total_chunk = total_chunk & MULTIPARTINFO_TOTALBITS;

    return (uint8_t)(_current_chunk | _total_chunk);

}

// Subsystems
void REQ_VERSION_SUBSYS(uint8_t *packet, uint8_t length) {
    // SPEC
    // Grab Version Number of Subsystem
    // PacketFormat = [Subsystem, Data]

    // Data Start Index = SINGLEPART_OFFSET
    // Incoming Data:
    // Data = []
    // Outgoing Data:
    // Data = [HIDPROC_MAJOR, HIDPROC_MINOR, HIDPROC_REVISION]
    packet[SINGLEPART_OFFSET] = HIDPROC_MAJOR;
    packet[SINGLEPART_OFFSET+1] = HIDPROC_MINOR;
    packet[SINGLEPART_OFFSET+2] = HIDPROC_REVISION;
    raw_hid_send(packet, length);
}

void SET_HSV_COLORS_SUBSYS(uint8_t *packet, uint8_t length) {
    // SPEC
    // Set HSV Colors
    // PacketFormat = [Subsystem, Data]

    // Data Start Index = SINGLEPART_OFFSET
    // Incoming Data:
    // Data = [Hue, Saturation, Value]
    // Outgoing Data:
    // Data = []
    uint8_t h = packet[SINGLEPART_OFFSET];
    uint8_t s = packet[SINGLEPART_OFFSET+1];
    uint8_t v = packet[SINGLEPART_OFFSET+2];
    rgb_matrix_sethsv(h, s, v);
}

void GET_HSV_COLORS_SUBSYS(uint8_t *packet, uint8_t length) {
    // SPEC
    // Grab HSV Colors
    // PacketFormat = [Subsystem, Data]

    // Data Start Index = SINGLEPART_OFFSET
    // Incoming Data:
    // Data = []
    // Outgoing Data:
    // Data = [Hue, Saturation, Value]
    packet[SINGLEPART_OFFSET] = rgb_matrix_get_hue();
    packet[SINGLEPART_OFFSET+1] = rgb_matrix_get_sat();
    packet[SINGLEPART_OFFSET+2] = rgb_matrix_get_val();
    raw_hid_send(packet,length);
}


void raw_hid_receive(uint8_t *packet, uint8_t length) {

    // EACH SUBSYSTEM IS RESPONSIBLE FOR SENDING IT'S OWN RAW_HID MESSAGE
    switch (packet[0]) { // Grab Subsystem
        case REQ_VERSION:
            REQ_VERSION_SUBSYS(packet, length);
            break;
        case SET_HSV_COLORS:
            SET_HSV_COLORS_SUBSYS(packet, length);
            break;
        case GET_HSV_COLORS:
            GET_HSV_COLORS_SUBSYS(packet, length);
            break;
        default:
            break;
    }
}