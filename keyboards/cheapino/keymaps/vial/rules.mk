SRC += chordal_hold.c
VIA_ENABLE = yes
VIAL_ENABLE = yes
TAP_DANCE_ENABLE = yes
COMBO_ENABLE = yes
ENCODER_MAP_ENABLE = yes

# P3(a): source is authoritative for tap-hold behaviour. Also required so
# that chordal_hold.c's get_chordal_hold() does not collide with the strong,
# unconditional definition qmk_settings.c compiles in when this is "yes".
QMK_SETTINGS = no
