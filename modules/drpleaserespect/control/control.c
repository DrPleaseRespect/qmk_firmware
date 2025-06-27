/* Copyright 2020 Kasper
 * Copyright 2024-2025 DrPleaseRespect <DrPleaseRespect@outlook.com>
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

/* Partially Based on OpenRGB's RawHID Implementation */

#include QMK_KEYBOARD_H
#include "raw_hid.h"
#include "control.h"
#include "print.h"

#ifdef RGB_MATRIX_ENABLE
#include "rgb_modes.h"
#endif

uint8_t volume_level = 0;

uint8_t get_volume(void) {
    return volume_level;
}

void send_message(const uint8_t *data, uint8_t length, uint8_t subsystem) {
    static uint8_t packet_id = 0;
    uint8_t total_length = length;
    uint8_t remaining_data = total_length;
    raw_packet packet;

    packet_id++;
    packet.subsystem = subsystem;
    packet.length = total_length;
    packet.id = packet_id;
    while (remaining_data > 0) {
        uint8_t data_offset = total_length - remaining_data;
        uint8_t bytes_to_copy = remaining_data < DATA_SIZE ? remaining_data : DATA_SIZE;
 
        memset(packet.data, 0, DATA_SIZE);
        memcpy(packet.data, &(data[data_offset]), bytes_to_copy);

        remaining_data -= bytes_to_copy;
        printf("RemainingData: %u\n", remaining_data);
        printf("DataLength: %u", total_length);
        raw_hid_send((uint8_t *)&packet, PACKET_SIZE);
    }
}

void send_error(uint8_t subsystem, uint8_t error_code) {
    uint8_t data[] = {subsystem, error_code};
    send_message(data, 2, ERROR_STATE);

}

__attribute__ ((weak)) void user_msg_handler(uint8_t *data, const raw_packet *packet, uint8_t length) {
    // All External Systems go through here (intended to be added to keymap and managed there)
    return;
}



void msg_handler(uint8_t *data, const raw_packet *packet, uint8_t length) {
    // packet->data != data (if multipart)
    //

    printf("\nRAWHID DATA\n");
    printf("Subsystem: %d\nPacketID: %d\nLength: %d\n[", packet->subsystem, packet->id, packet->length);
    for (int i = 0; i < packet->length; ++i)
    {
        printf("%d,", data[i]);
    }
    printf("]\n");

    switch (packet->subsystem) {
        case GET_PROTOCOL_INFO:
            data[0] = GET_PROTOCOL_INFO;
            data[1] = CTRL_VERSION_MAJOR;
            data[2] = CTRL_VERSION_MINOR;
            memcpy(&data[3], "SHIKIPROTOCOL", 13);
            send_message(data, 16, GET_PROTOCOL_INFO);
            break;



#ifdef RGB_MATRIX_ENABLE
        case SET_RGB_SETTINGS:
            // [H, S, V, SPEED, MODE, SAVE]
            // 0-255 Range
            
            // Expected Length
            if (length != 6) {
                return;
            }


            uint8_t h = data[0];
            uint8_t s = data[1];
            uint8_t v = data[2];
            uint8_t speed = data[3];
            uint8_t mode = data[4];
            bool save = (data[5] > 0);

            if (save) {
                rgb_matrix_mode(mode);
                rgb_matrix_set_speed(speed);
                rgb_matrix_sethsv(h,s,v);
            } else {
                rgb_matrix_mode_noeeprom(mode);
                rgb_matrix_set_speed_noeeprom(speed);
                rgb_matrix_sethsv_noeeprom(h,s,v);
            }
            break;

        case GET_RGB_MODES:
            memset(data, 0, MAX_DATA_SIZE);
            const uint8_t mode_size = sizeof(enabled_rgb_matrix_effect_ids) / sizeof(enabled_rgb_matrix_effect_ids[0]);
            if (mode_size > MAX_DATA_SIZE) {
                // Send Error State if mode_size is greater than MAX_DATA_SIZE 
                send_error(GET_RGB_MODES, 0);
                break;
            }
            for (int i = 0; i < mode_size; i++)
            {
                data[i] = enabled_rgb_matrix_effect_ids[i];                    
            }
            send_message(data, mode_size, GET_RGB_MODES);
            break;
#endif
        case VOLUME:
            // Packet Data [volume level]
            volume_level = data[0];
            break;

        // 100-255 range is for external stuff
        case SUBSYS_SAFERANGE...SUBSYS_MAXRANGE:
            user_msg_handler(data, packet, length);
            break;
        default:
            break;
    }

}



void raw_hid_receive(uint8_t *raw_packet_data, uint8_t length) {
    // Only used for multipart messages
    static uint8_t message_data[MAX_DATA_SIZE];
    static bool multipart_packet = false;
    static uint8_t last_packetid = 0;
    static uint8_t remaining_data = 0;
    const raw_packet *packet;


    packet = (const raw_packet*)raw_packet_data;


    // Detection of multipart packet
    if ((packet->length) > DATA_SIZE) {
        multipart_packet = true;
    } else {
        multipart_packet = false;
    }

    if (packet->length > MAX_DATA_SIZE) {
        printf("Error: Packet length %u exceeds maximum data size %u\n", packet->length, MAX_DATA_SIZE);
        return;  // Exit if data is too large
    }

    if (!multipart_packet) {
        // Start Handling of Singlepart Packet


        // Clean message_data buffer
        memset(&message_data[0], 0, MAX_DATA_SIZE);
        // Copy packet data to message_data buffer
        memcpy(&message_data[0], &(packet->data[0]), DATA_SIZE);
    

        // Pass message data to msg_handler
        msg_handler(message_data, packet, packet->length);
        return;
    }

    if (multipart_packet) {

        // Start Handling of Multipart Packet
        if (last_packetid != packet->id || remaining_data == 0) {
            if (last_packetid != packet->id) {
                printf("PacketID %u != %u\n", last_packetid, packet->id);
            }
            if (remaining_data != 0) {
                printf("RemaingData is not zero! Abandoning Previous Transmission!\n");
            }
            multipart_packet = true;
            last_packetid = packet->id;
            remaining_data = packet->length;
            // Last packet ID is not the same as current packet ID
            // Assume Start of new packet

            // Clean data
            memset(&message_data[0], 0, MAX_DATA_SIZE);

            // Copy data from packet to data array
            memcpy(&message_data[0], &(packet->data[0]), DATA_SIZE);

            // Remove DATA_SIZE from remaining_data
            remaining_data -= DATA_SIZE;

        } else {
            // Last packet ID is the same as current packet ID
            // Assume continuation of last packet

            if (remaining_data > 0) {
                // Calculate Data Offset
                uint8_t data_offset = packet->length - remaining_data;
                //printf("DataOffset %u\n", data_offset);

                uint8_t bytes_to_copy = remaining_data < DATA_SIZE ? remaining_data : DATA_SIZE;
                memcpy(&message_data[data_offset], &(packet->data[0]), bytes_to_copy);

                // Update remaining data
                remaining_data -= bytes_to_copy;
            }
        }

        if (remaining_data == 0) {
            msg_handler(message_data, packet, packet->length);
        }
    }
    

}

