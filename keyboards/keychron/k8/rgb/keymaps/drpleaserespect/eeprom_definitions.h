#pragma once

typedef union {
	uint32_t raw;
	struct {
		bool pass_sys_unlocked :1;
        #ifdef BLUETOOTH_ENABLE
        uint8_t bt_profile;
        #endif
	};
} user_config_t;

user_config_t eeprom_config;
