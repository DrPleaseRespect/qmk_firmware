VIA_ENABLE = no
TAP_DANCE_ENABLE = no
ENCODER_MAP_ENABLE = yes
AUTOCORRECT_ENABLE = yes
COMBO_ENABLE = yes
MOUSEKEY_ENABLE = yes
TOP_SYMBOLS = yes
LTO_ENABLE = no
CONSOLE_ENABLE = no


VPATH += keyboards/keychron/common
#SRC += keychron_common.c
SRC += ./features/gamemode/gamemode.c
SRC += ./features/encoder_spinner/encoder_spinner.c