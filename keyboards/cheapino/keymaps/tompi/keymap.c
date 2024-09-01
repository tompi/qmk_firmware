#include QMK_KEYBOARD_H
#if __has_include("keymap.h")
#    include "keymap.h"
#endif



enum custom_keycodes {
    SMTD_KEYCODES_BEGIN = SAFE_RANGE,
    CKC_A, // reads as C(ustom) + KC_A, but you may give any name here
    CKC_R,
    CKC_S,
    CKC_T,
    CKC_N,
    CKC_E,
    CKC_I,
    CKC_O,
    SMTD_KEYCODES_END
};

#include "sm_td.h"

uint32_t get_smtd_timeout(uint16_t keycode, smtd_timeout timeout) {
    if (keycode == CKC_N && timeout == SMTD_TIMEOUT_RELEASE) return 35;

    return get_smtd_timeout_default(timeout);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_smtd(keycode, record)) {
        return false;
    }
    return true;
}

void on_smtd_action(uint16_t keycode, smtd_action action, uint8_t tap_count) { 
    switch (keycode) {
        SMTD_MT(CKC_A, KC_A, KC_LEFT_GUI)
        SMTD_MT(CKC_R, KC_R, KC_LEFT_ALT)
        SMTD_MT(CKC_S, KC_S, KC_LEFT_CTRL)
        SMTD_MT(CKC_T, KC_T, KC_LSFT)
        SMTD_MT(CKC_N, KC_N, KC_RSFT)
        SMTD_MT(CKC_E, KC_E, KC_RIGHT_CTRL)
        SMTD_MT(CKC_I, KC_I, KC_RIGHT_ALT)
        SMTD_MT(CKC_O, KC_O, KC_RIGHT_GUI)
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_split_3x5_3(KC_Q, KC_W, KC_F, KC_P, KC_B, KC_J, KC_L, KC_U, KC_Y, KC_SCLN, CKC_A, CKC_R, CKC_S, CKC_T, KC_G, KC_M, CKC_N, CKC_E, CKC_I, CKC_O, KC_Z, KC_X, KC_C, KC_D, LT(5,KC_V), LT(1,KC_K), KC_H, KC_COMM, KC_DOT, KC_SLSH, LT(6,KC_ESC), LT(3,KC_SPC), LT(5,KC_TAB), LT(1,KC_DEL), LT(2,KC_BSPC), LT(4,KC_ENT)),
    [1] = LAYOUT_split_3x5_3(KC_VOLU, KC_WH_L, KC_MS_U, KC_WH_U, KC_WH_R, KC_NO, KC_NO, KC_NO, KC_NO, QK_RBT, KC_MS_L, KC_BTN2, KC_BTN1, KC_MS_R, KC_MUTE, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_VOLD, KC_MNXT, KC_MS_D, KC_WH_D, KC_MPLY, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO),
    [2] = LAYOUT_split_3x5_3(NK_TOGG, KC_HOME, KC_DEL, KC_PGUP, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_LEFT, KC_UP, KC_RGHT, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_END, KC_DOWN, KC_PGDN, KC_NO, KC_NO, KC_BTN1, KC_BTN2, KC_BTN3, KC_BTN4, KC_NO, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO),
    [3] = LAYOUT_split_3x5_3(KC_NO, KC_NO, KC_NO, KC_NO, KC_WH_U, KC_AT, KC_UNDS, KC_PIPE, KC_GRV, KC_PERC, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RGB_TOG, KC_HASH, KC_TAB, KC_EXLM, KC_DQUO, KC_DLR, KC_BTN4, KC_BTN3, KC_BTN2, KC_BTN1, KC_WH_D, KC_TILD, KC_QUOT, KC_BSLS, KC_SLSH, KC_AMPR, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_NO),
    [4] = LAYOUT_split_3x5_3(KC_GRV, KC_CIRC, KC_LT, KC_GT, KC_SCLN, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_LCBR, KC_RCBR, KC_LPRN, KC_RPRN, KC_AT, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MINS, KC_EXLM, KC_LBRC, KC_RBRC, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_NO),
    [5] = LAYOUT_split_3x5_3(QK_RBT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_F7, KC_F8, KC_F9, KC_F10, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_F4, KC_F5, KC_F6, KC_F11, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_F1, KC_F2, KC_F3, KC_F12, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
    [6] = LAYOUT_split_3x5_3(RGB_SPI, RGB_VAI, RGB_SAI, RGB_HUI, RGB_MOD, KC_PPLS, KC_P7, KC_P8, KC_P9, KC_PAST, EE_CLR, KC_TRNS, KC_TRNS, KC_TRNS, RGB_TOG, KC_PMNS, KC_P4, KC_P5, KC_P6, KC_PSLS, RGB_SPD, RGB_VAD, RGB_SAD, RGB_HUD, RGB_RMOD, KC_PDOT, KC_P1, KC_P2, KC_P3, KC_PEQL, KC_NO, KC_NO, KC_NO, KC_0, KC_COMM, KC_P0)
};

#if defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {

};
#endif // defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)
