// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
     /*
      * ┌───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┐
      * │ Q │ W │ E │ R │ T │       │ Y │ U │ I │ O │ P │
      * ├───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┤
      * │ A │ S │ D │ F │ G │       │ H │ J │ K │ L │ ; │
      * ├───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┤
      * │ Z │ X │ C │ V │ B │       │ N │ M │ , │ . │ / │
      * └───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┘
      *           ┌───┐                   ┌───┐
      *           │GUI├───┐           ┌───┤Alt│
      *           └───┤Bsp├───┐   ┌───┤Ent├───┘
      *               └───┤   │   │   ├───┘
      *                   └───┘   └───┘
      *
      * 
      * 
        KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,                                           KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,     KC_TRANSPARENT,
        KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,                                           KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,     KC_TRANSPARENT,
        KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,                                           KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,    KC_TRANSPARENT,
                                                                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,           KC_TRANSPARENT,  KC_TRANSPARENT,  KC_TRANSPARENT
      */
    [0] = LAYOUT_split_3x5_3(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                                         KC_Y,    KC_U,    KC_I,    KC_O,          KC_P,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                         KC_H,    KC_J,    KC_K,    LT(2,KC_L),    LT(3,KC_SCLN),
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                         KC_N,    KC_M,    KC_COMM, KC_DOT,        KC_SLSH,
                                      KC_LCTL, LT(1,KC_SPACE), OSM(MOD_LSFT),           KC_LGUI,  KC_RCTL,  KC_RALT
    ),
    
    [1] = LAYOUT_split_3x5_3(
        KC_EXLM,           KC_AT,             KC_LCBR,           KC_RCBR,           KC_PIPE,                                             KC_TRANSPARENT,    KC_7,    KC_8,    KC_9,     KC_ASTR,
        KC_HASH,           KC_DLR,            KC_LPRN,           KC_RPRN,           KC_DOT,                                              KC_TRANSPARENT,    KC_4,    KC_5,    KC_6,     KC_PLUS,
        KC_SLASH,          KC_CIRC,           KC_LBRC,           KC_RBRC,           KC_TILD,                                             KC_TRANSPARENT,    KC_1,    KC_2,    KC_3,    KC_MINUS,
                                                                      KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,           KC_0,  KC_0,  KC_DOT
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
};

const uint16_t PROGMEM combo0[] = { KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM combo1[] = { KC_M, KC_COMMA, COMBO_END};
const uint16_t PROGMEM combo2[] = { KC_D, KC_F, COMBO_END};
const uint16_t PROGMEM combo3[] = { KC_1, KC_2, COMBO_END};
const uint16_t PROGMEM combo4[] = { KC_Q, KC_W, COMBO_END};
const uint16_t PROGMEM combo5[] = { KC_E, KC_R, COMBO_END};
const uint16_t PROGMEM combo6[] = { KC_LPRN, KC_RPRN, COMBO_END};

combo_t key_combos[] = {
    COMBO(combo0, KC_BSPC),
    COMBO(combo1, KC_ENTER),
    COMBO(combo2, KC_DELETE),
    COMBO(combo3, KC_ENTER),
    COMBO(combo4, KC_ESCAPE),
    COMBO(combo5, KC_TAB),
    COMBO(combo6, KC_DOT),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] = { ENCODER_CCW_CW(KC_PGUP, KC_PGDN),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [1] = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI),  ENCODER_CCW_CW(RGB_SAD, RGB_SAI)  },
    [2] = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI),  ENCODER_CCW_CW(RGB_SPD, RGB_SPI)  },
    [3] = { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD), ENCODER_CCW_CW(KC_LEFT, KC_RIGHT) },
};
#endif