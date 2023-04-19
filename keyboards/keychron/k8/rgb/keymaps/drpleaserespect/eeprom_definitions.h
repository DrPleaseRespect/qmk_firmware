#pragma once

typedef union {
	uint32_t raw;
	struct {
		bool pass_sys_unlocked :1;
	};
} user_config_t;

user_config_t eeprom_config;
