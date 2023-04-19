#ifdef RGB_MATRIX_ENABLE

#include "rgb_matrix.h"
#include "config.h"

// readability
#define NA NO_LED
/* ANSI keymap LEDs */
led_config_t g_led_config = { {
    {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18 },
    { 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, NA, 33, 34, 35, 36 },
    { 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, NA, 51, 52, 53, 54 },
    { 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, NA, 67, NA, 68, 69, 70, NA },
    { 71, NA, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, NA, 82, 83, 84, 85, 86, 87 },
    { 88, 89, 90, NA, NA, NA, 91, NA, NA, NA, 92, 93, 94, 95, 96, 97, 98, 99, NA }
}, {
    {0  ,  0}, {12 ,  0}, {25 ,  0}, {37 ,  0}, {50 ,  0}, {62 ,  0}, {75 ,  0}, {87 ,  0}, {100,  0}, {112,  0}, {124,  0}, {137,  0}, {149,  0}, {162,  0}, {174,  0}, {187,  0}, {199,  0}, {212,  0}, {224,  0},
    {0  , 13}, {12 , 13}, {25 , 13}, {37 , 13}, {50 , 13}, {62 , 13}, {75 , 13}, {87 , 13}, {100, 13}, {112, 13}, {124, 13}, {137, 13}, {149, 13}, {168, 13},            {187, 13}, {199, 13}, {212, 13}, {224, 13},
    {3  , 26}, {19 , 26}, {31 , 26}, {44 , 26}, {56 , 26}, {68 , 26}, {81 , 26}, {93 , 26}, {106, 26}, {118, 26}, {131, 26}, {143, 26}, {156, 26}, {168, 26},            {187, 26}, {199, 26}, {212, 26}, {224, 32},
    {5  , 38}, {22 , 38}, {34 , 38}, {47 , 38}, {59 , 38}, {72 , 38}, {84 , 38}, {96 , 38}, {109, 38}, {121, 38}, {134, 38}, {146, 38},            {166, 38},            {187, 38}, {199, 38}, {212, 38},
    {8  , 51},            {28 , 51}, {40 , 51}, {53 , 51}, {65 , 51}, {78 , 51}, {90 , 51}, {103, 51}, {115, 51}, {128, 51}, {140, 51},            {157, 51}, {174, 51}, {187, 51}, {199, 51}, {212, 51}, {224, 58},
    {2  , 64}, {17 , 64}, {33 , 64},                                  {79 , 64},                                  {124, 64}, {137, 64}, {149, 64}, {162, 64}, {174, 64}, {187, 64}, {199, 64}, {212, 64},
}, {
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,    4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,    4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,    4,    4, 4, 4,
    4,    4, 4, 4, 4, 4, 4, 4, 4, 4, 4,    4, 4, 4, 4, 4, 4,
    4, 4, 4,          4,          4, 4, 4, 4, 4, 4, 4, 4,
} };
#endif
