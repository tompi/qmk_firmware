// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
     /*
        KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,                                           KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,     KC_TRANSPARENT,
        KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,                                           KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,     KC_TRANSPARENT,
        KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,                                           KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,
                                                                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,           KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT
      */
    [0] = LAYOUT_split_3x5_3(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                                         KC_Y,    KC_U,    KC_I,    KC_O,          KC_P,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                         KC_H,    KC_J,    KC_K,    LT(2,KC_L),    LT(3,KC_SCLN),
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                         KC_N,    KC_M,    KC_COMM, KC_DOT,        KC_SLSH,
                                      KC_LCTL, LT(1,KC_SPACE), OSM(MOD_LSFT),       KC_F,  KC_RCTL,  KC_RALT
    ),

    [1] = LAYOUT_split_3x5_3(
        KC_EXLM,           KC_AT,             KC_LCBR,           KC_RCBR,           KC_PIPE,                                             KC_TRNS,    KC_7,    KC_8,    KC_9,     KC_ASTR,
        KC_HASH,           KC_DLR,            KC_LPRN,           KC_RPRN,           KC_DOT,                                              KC_TRNS,    KC_4,    KC_5,    KC_6,     KC_PLUS,
        KC_SLASH,          KC_CIRC,           KC_LBRC,           KC_RBRC,           KC_TILD,                                             KC_TRNS,    KC_1,    KC_2,    KC_3,    KC_MINUS,
                                                                      KC_TRNS, KC_TRNS, KC_TRNS,           KC_0,  KC_0,  KC_DOT
    ),

    [2] = LAYOUT_split_3x5_3(
        KC_TRANSPARENT,    KC_PGDN,           KC_UP,             KC_PGUP,           KC_TRANSPARENT,                                           KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,     KC_TRANSPARENT,
        KC_TRANSPARENT,    KC_LEFT,           KC_DOWN,           KC_RIGHT,          KC_TRANSPARENT,                                           KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,     KC_TRANSPARENT,
        KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,                                           KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,
                                                                          KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,            KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT
    ),

    [3] = LAYOUT_split_3x5_3(
        KC_TRANSPARENT,    KC_MS_WH_DOWN,    KC_MS_UP,        KC_MS_WH_UP,    KC_TRANSPARENT,                                           KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,     KC_TRANSPARENT,
        KC_TRANSPARENT,    KC_MS_LEFT,       KC_MS_DOWN,      KC_MS_RIGHT,    KC_TRANSPARENT,                                           KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,     KC_TRANSPARENT,
        KC_TRANSPARENT,    KC_TRANSPARENT,   KC_TRANSPARENT,  KC_TRANSPARENT, KC_TRANSPARENT,                                           KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,     KC_TRANSPARENT,
                                                                        KC_MS_BTN2, KC_MS_BTN1, KC_TRANSPARENT,           KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT
    ),
    [4] = LAYOUT_split_3x5_3(
        KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,                                         KC_J,    KC_L,    KC_U,    KC_Y,          KC_SCLN,
        KC_A,    KC_R,    KC_S,    KC_T,    KC_D,                                         KC_H,    KC_N,    KC_E,    LT(2,KC_I),    LT(3,KC_O),
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                         KC_K,    KC_M,    KC_COMM, KC_DOT,        KC_SLSH,
                                      KC_LCTL, LT(1,KC_SPACE), OSM(MOD_LSFT),           KC_TRNS,  KC_RCTL,  KC_RALT
    ),
};

const uint16_t PROGMEM combo0[] = { KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM combo1[] = { KC_M, KC_COMMA, COMBO_END};
const uint16_t PROGMEM combo2[] = { KC_D, KC_F, COMBO_END};
const uint16_t PROGMEM combo3[] = { KC_1, KC_2, COMBO_END};
const uint16_t PROGMEM combo4[] = { KC_Q, KC_W, COMBO_END};
const uint16_t PROGMEM combo5[] = { KC_E, KC_R, COMBO_END};
const uint16_t PROGMEM combo6[] = { KC_LPRN, KC_RPRN, COMBO_END};
const uint16_t PROGMEM combo7[] = { KC_U, KC_I, COMBO_END};
const uint16_t PROGMEM combo8[] = { KC_RCTL, KC_RALT, COMBO_END};


combo_t key_combos[] = {
    COMBO(combo0, KC_BSPC),
    COMBO(combo1, KC_ENTER),
    COMBO(combo2, KC_DELETE),
    COMBO(combo3, KC_ENTER),
    COMBO(combo5, KC_TAB),
    COMBO(combo6, KC_DOT),
    COMBO(combo7, KC_QUOTE),
    COMBO(combo8, TG(4)),
};



