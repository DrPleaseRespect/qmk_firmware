#pragma once

enum raw_hid_functions {
  HID_SET_COLOR = 1,
  HID_SET_SPEED,
  HID_SET_MODE,
  HID_GET_AVAILABLE_MODES,
  HID_GET_CURRENT_RGB_SETTINGS,
  HID_SAVE_RGB_SETTINGS,
  HID_VOLUME,
};


#define VolumeSTART 17
#define RAW_EPSIZE 32

// CODE BREAKS IF RAW_EPSIZE IS TOO SMALL 
// (might increase in the future)
#if RAW_EPSIZE < 3
#   error RAW_EPSIZE TOO SMALL! (>2)
#endif
uint8_t SystemVolume;
void VolumeLevelIndicator(bool pressed);
void disable_rgb_tracked(bool status);