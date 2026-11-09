// Copyright 2024 (@getreu)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#ifdef VIAL_COMBO_ENABLE
#include "vial.h"
#include "dynamic_keymap.h"
#endif

#ifdef VIAL_TAP_DANCE_ENABLE
#include "vial.h"
#include "dynamic_keymap.h"
#endif

/**
 * VIAL-OPTIMIZED KEYMAP FOR CHEAPINO
 *
 * This is a Vial-friendly version based on the musclememory keymap.
 * Tap dance and combos are configured via Vial's dynamic system.
 * Default values are loaded on EEPROM reset via eeconfig_init_user().
 */

/**
 * RGBLIGHT CONFIGURATION & STORAGE
 * Capture and restore dynamic color from EEPROM.
 */
// Define a custom structure to group HSV values
typedef struct {
    uint8_t h;
    uint8_t s;
    uint8_t v;
} custom_hsv_t;
static custom_hsv_t old_color;

/**
 * LAYER DEFINITIONS
 */
enum layers { _BASE = 0, _L1, _L2, _L3, _L4, _L5, _L6, _L7 };

/**
 * COLOR DEFINITIONS
 */
#define COLOR_AMBER     20, 180, 10
#define COLOR_MINT      45, 140, 10
#define COLOR_CYAN      140, 200, 10
#define COLOR_LAVENDER  210, 130, 15
#define COLOR_RED       0, 255, 12
#define COLOR_PINK      230, 170, 10
#define COLOR_WHITE     0, 0, 10

/**
 * Initialize RGB color capture
 */
void keyboard_post_init_user(void) {
    // Force static light, so that an animation mode stored in EEPROM cannot
    // fight the per-layer colours in layer_state_set_user().
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);

    // Seed the base layer colour from the persisted RGB settings.  Without
    // this, old_color stays {0,0,0} and the base layer goes dark the first
    // time we return to it.
    old_color.h = rgblight_get_hue();
    old_color.s = rgblight_get_sat();
    old_color.v = rgblight_get_val();
}

/**
 * LAYER VISUAL FEEDBACK
 * Changes RGB color based on active layer
 */
layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case _L2:
            rgblight_sethsv_noeeprom(COLOR_RED);
            break;
        case _L3:
            rgblight_sethsv_noeeprom(COLOR_CYAN);
            break;
        case _L4:
            rgblight_sethsv_noeeprom(COLOR_LAVENDER);
            break;
        case _L5:
            rgblight_sethsv_noeeprom(COLOR_AMBER);
            break;
        case _L6:
            rgblight_sethsv_noeeprom(COLOR_MINT);
            break;
        case _L7:
            rgblight_sethsv_noeeprom(COLOR_PINK);
            break;
        default:
            // Restore original color on base layer
            rgblight_sethsv_noeeprom(old_color.h, old_color.s, old_color.v);
            break;
    }
    return state;
}

/**
 * Keep old_color mirroring the live colour while on _L7, so that leaving the
 * layer restores exactly what it was showing.
 *
 * This can't be done in post_process_record_user() for the underglow hue/
 * saturation/value keys: process_underglow() returns false after applying each of those
 * (the normal QMK pattern for a keycode that needs no further
 * register/unregister handling), and QMK's process_record() does an early
 * return on a false result -- skipping post_process_record_quantum(), and
 * therefore post_process_record_user(), entirely for that keypress. Polling
 * here is the one hook that still sees the change before the layer is left.
 */
void matrix_scan_user(void) {
    if (get_highest_layer(layer_state) == _L7) {
        old_color.h = rgblight_get_hue();
        old_color.s = rgblight_get_sat();
        old_color.v = rgblight_get_val();
    }
}

/**
 * EEPROM INITIALIZATION - Set Default Combos, Tap Dances and Encoder Map
 * This runs when EEPROM is reset (first flash or reset command).
 *
 * Pattern: set up defaults just like in musclememory, but using Vial's
 * dynamic system -- combos, tap dances and the encoder map are stored in
 * EEPROM and can be reconfigured via the Vial GUI.  RGB is deliberately not
 * seeded here: keyboard_post_init_user() already reads whatever is
 * persisted, and duplicating that here would fight it.
 */
#if defined(VIAL_COMBO_ENABLE) || defined(VIAL_TAP_DANCE_ENABLE)
void eeconfig_init_user(void) {

#ifdef VIAL_COMBO_ENABLE
    // 2. Load Dynamic Combos (These write to EEPROM)
    vial_combo_entry_t combo = {0};

    // Combo 0: V + Space -> Layer 7
    combo.input[0] = RGUI_T(KC_V);
    combo.input[1] = LGUI_T(KC_SPC);
    combo.output = MO(_L7);
    dynamic_keymap_set_combo(0, &combo);

    // Combo 1: M + Enter -> Layer 7
    combo.input[0] = RGUI_T(KC_M);
    combo.input[1] = LGUI_T(KC_ENT);
    combo.output = MO(_L7);
    dynamic_keymap_set_combo(1, &combo);

    // Combo 2: ESC + Backspace -> Go to Layer 3
    combo.input[0] = LT(_L6, KC_ESC);
    combo.input[1] = LT(_L5, KC_BSPC);
    combo.output = TO(_L3);
    dynamic_keymap_set_combo(2, &combo);

    // Combo 5: TD(2) + Backspace -> Go to Layer 3 (stay in symbol layer from L3)
    combo.input[0] = TD(2);
    combo.input[1] = LT(_L5, KC_BSPC);
    combo.output = TO(_L3);
    dynamic_keymap_set_combo(5, &combo);

    // Combo 3 & 4: Outer thumbs -> Caps Lock
    combo.input[0] = OSL(_L4); combo.input[1] = TD(0);
    combo.output = KC_CAPS_LOCK;
    dynamic_keymap_set_combo(3, &combo);

    combo.input[0] = OSL(_L4); combo.input[1] = TD(1);
    combo.output = KC_CAPS_LOCK;
    dynamic_keymap_set_combo(4, &combo);

    // Combo 6: Comma + Dot -> Go to Layer 2 (numpad)
    combo.input[0] = KC_COMM;
    combo.input[1] = KC_DOT;
    combo.output = TO(_L2);
    dynamic_keymap_set_combo(6, &combo);

#endif

#ifdef VIAL_TAP_DANCE_ENABLE
    // 4. Load Dynamic Tap Dances
    vial_tap_dance_entry_t td = {0};

    // Excerpt from: void eeconfig_init_user( )
    // TD 0: APP / MO(_L2) momentary / TG(_L2) toggle
    td.on_tap = KC_APP;
    td.on_hold = MO(_L2);           // momentary L2
    td.on_double_tap = TO(_L2);
    td.on_tap_hold = KC_NO;
    td.custom_tapping_term = 350;
    dynamic_keymap_set_tap_dance(0, &td);

    // TD 1: TO(0) / MO(_L2) momentary / TG(_L2) toggle
    td.on_tap = TO(0);
    td.on_hold = MO(_L2);           // momentary L2
    td.on_double_tap = TO(_L2);
    td.on_tap_hold = KC_NO;
    td.custom_tapping_term = 350;
    dynamic_keymap_set_tap_dance(1, &td);

    // TD 2: TO(0) / MO(_L6)
    td.on_tap = TO(0); td.on_hold = MO(_L6); td.on_double_tap = KC_NO; td.on_tap_hold = KC_NO;
    td.custom_tapping_term = 350;
    dynamic_keymap_set_tap_dance(2, &td);

    // TD 3: KC_KP_PLUS (tap) / KC_PAST (hold) / KC_KP_PLUS (tap-hold, repeating)
    td.on_tap = KC_KP_PLUS; td.on_hold = KC_PAST; td.on_double_tap = KC_NO; td.on_tap_hold = KC_KP_PLUS;
    td.custom_tapping_term = 200;
    dynamic_keymap_set_tap_dance(3, &td);

    // TD 4: KC_KP_MINUS (tap) / KC_PSLS (hold) / KC_KP_MINUS (tap-hold, repeating)
    td.on_tap = KC_KP_MINUS; td.on_hold = KC_PSLS; td.on_double_tap = KC_NO; td.on_tap_hold = KC_KP_MINUS;
    td.custom_tapping_term = 200;
    dynamic_keymap_set_tap_dance(4, &td);

    // TD 5: KC_KP_DOT (tap) / KC_PCMM (hold) / KC_KP_DOT (tap-hold, repeating)
    td.on_tap = KC_KP_DOT; td.on_hold = KC_PCMM; td.on_double_tap = KC_NO; td.on_tap_hold = KC_KP_DOT;
    td.custom_tapping_term = 200;
    dynamic_keymap_set_tap_dance(5, &td);

    // TD 6: KC_KP_0 (tap) / KC_PEQL (hold) / KC_KP_0 (tap-hold, repeating)
    td.on_tap = KC_KP_0; td.on_hold = KC_PEQL; td.on_double_tap = KC_NO; td.on_tap_hold = KC_KP_0;
    td.custom_tapping_term = 200;
    dynamic_keymap_set_tap_dance(6, &td);
#endif

#ifdef ENCODER_MAP_ENABLE
    // 5. Load Dynamic Encoder Map.  This is the part that actually runs on
    // this keymap -- see the comment on the static encoder_map[] above.
    // Volume everywhere, brightness on _L7, matching musclememory.
    for (uint8_t layer = 0; layer < DYNAMIC_KEYMAP_LAYER_COUNT; layer++) {
        dynamic_keymap_set_encoder(layer, 0, false, KC_TRNS);
        dynamic_keymap_set_encoder(layer, 0, true, KC_TRNS);
    }
    dynamic_keymap_set_encoder(_BASE, 0, false, KC_VOLD);
    dynamic_keymap_set_encoder(_BASE, 0, true, KC_VOLU);
    dynamic_keymap_set_encoder(_L7, 0, false, KC_BRID);
    dynamic_keymap_set_encoder(_L7, 0, true, KC_BRIU);
#endif

    // Finalize the sync
    vial_init();
}
#endif

/**
 * KEYMAP DATA
 * Layout for each layer
 *
 * Note: Right outer thumb uses TD(0) - configured in eeconfig_init_user() above
 *       Combos are also configured in eeconfig_init_user() to match musclememory defaults
 */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_BASE] = LAYOUT(
    KC_Q,         KC_W,         KC_E,         KC_R,         KC_T,         KC_MUTE,  KC_Y,         KC_U,         KC_I,         KC_O,         KC_P,
    LSFT_T(KC_A), RALT_T(KC_S), LT(_L3,KC_D), LCTL_T(KC_F), LALT_T(KC_G),           LALT_T(KC_H), RCTL_T(KC_J), LT(_L3,KC_K), RALT_T(KC_L), RSFT_T(KC_SCLN),
    KC_Z,         KC_X,         KC_C,         RGUI_T(KC_V), KC_B,                   KC_N,         RGUI_T(KC_M), KC_COMM,      KC_DOT,       KC_SLSH,
    OSL(_L4),     LT(_L6,KC_ESC), LGUI_T(KC_SPC),                                   LGUI_T(KC_ENT), LT(_L5,KC_BSPC), TD(0)
),

[_L1] = LAYOUT(
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS,                     KC_TRNS, KC_TRNS, KC_TRNS
),

[_L2] = LAYOUT(
    KC_TRNS,        KC_TRNS,        KC_TRNS,         KC_TRNS,        KC_TRNS, KC_TRNS, TD(3),              KC_KP_7,         KC_KP_8,          KC_KP_9,         TD(6),
    KC_TRNS,        KC_TRNS,        KC_TRNS,         KC_TRNS,        KC_TRNS,         LALT_T(KC_NUM_LOCK), RCTL_T(KC_KP_4), LT(_L3, KC_KP_5), RALT_T(KC_KP_6), RSFT_T(KC_PENT),
    KC_TRNS,        KC_TRNS,        KC_TRNS,         KC_TRNS,        KC_TRNS,         TD(4),               RGUI_T(KC_KP_1), KC_KP_2,          KC_KP_3,         TD(5),
    KC_TRNS, TD(2), KC_TRNS,                              KC_TRNS, KC_TRNS, TD(1)
),

[_L3] = LAYOUT(
    KC_1,           KC_2,           KC_3,             KC_4,           KC_5,  KC_TRNS,  KC_6,            KC_7,            KC_8,          KC_9,            KC_0,
    LSFT_T(KC_ESC), RALT_T(KC_INS), LT(_L3,KC_DEL),   LCTL_T(KC_TAB), LALT_T(KC_BSPC), LALT_T(KC_LEFT), RCTL_T(KC_DOWN), LT(_L3,KC_UP), RALT_T(KC_RGHT), RSFT_T(KC_ENT),
    KC_GRV,         KC_MINS,        KC_EQL,           RGUI_T(KC_LBRC), KC_RBRC,        KC_BSLS,         RGUI_T(KC_QUOT), KC_COMM,       KC_DOT,          KC_SLSH,
    KC_TRNS, TD(2), KC_TRNS,                              KC_TRNS, KC_TRNS, TD(1)
),

[_L4] = LAYOUT(
    KC_F1,          KC_F2,          KC_F3,   KC_F4,           KC_F5,   QK_REBOOT, KC_F6,          KC_F7,          KC_F8,   KC_F9,          KC_F10,
    LSFT_T(KC_F11), RALT_T(KC_F12), KC_F13,  LCTL_T(KC_F14),  LALT_T(KC_F15),     LALT_T(KC_F16), RCTL_T(KC_F17), KC_F18,  RALT_T(KC_F19), RSFT_T(KC_F20),
    KC_F21,         KC_F22,         KC_F23,  RGUI_T(KC_F24),  KC_NO,              KC_NO,          RGUI_T(KC_NO),  KC_NO,   KC_NO,          MO(_L7),
    KC_TRNS, TD(2), KC_TRNS,                              KC_TRNS, KC_TRNS, TD(1)
),

[_L5] = LAYOUT(
    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,  KC_HOME, KC_INS,  KC_DEL,  KC_END,  KC_BSPC,
    KC_ESC,  KC_INS,  KC_DEL,  KC_TAB,  KC_BSPC,          KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_ENT,
    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,            KC_NO,   KC_PGDN, KC_PGUP, KC_NO,   KC_NO,
    KC_TRNS, TD(2), KC_TRNS,                              KC_TRNS, KC_TRNS, TD(1)
),

[_L6] = LAYOUT(
    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  KC_NO,  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
    MS_BTN5, MS_BTN1, MS_BTN3, MS_BTN2, MS_BTN4,        MS_LEFT, MS_DOWN, MS_UP,   MS_RGHT, MS_BTN1,
    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,          MS_WHLL, MS_WHLD, MS_WHLU, MS_WHLR, KC_NO,
    KC_TRNS, TD(2), KC_TRNS,                            KC_TRNS, KC_TRNS, TD(1)
),

[_L7] = LAYOUT(
    QK_BOOT,   QK_CLEAR_EEPROM, QK_UNDERGLOW_VALUE_DOWN, QK_UNDERGLOW_VALUE_UP, QK_UNDERGLOW_TOGGLE, KC_NO, KC_NO,   KC_BRID, KC_BRIU, KC_NO, KC_NO,
    QK_REBOOT, QK_UNDERGLOW_SATURATION_DOWN,         QK_UNDERGLOW_HUE_DOWN, QK_UNDERGLOW_HUE_UP, QK_UNDERGLOW_SATURATION_UP,        KC_MUTE, KC_VOLD, KC_VOLU, KC_NO, KC_NO,
    KC_NO,     KC_NO,           KC_NO,   KC_NO,   KC_NO,          KC_MPLY, KC_MPRV, KC_MNXT, KC_NO, KC_NO,
    KC_TRNS, TD(2), KC_TRNS,                                    KC_TRNS, KC_TRNS, TD(1)
)
};

/**
 * ENCODER
 * Static factory-default map, same as musclememory. On this keymap it is
 * NOT what actually runs: with VIAL_ENABLE, quantum/dynamic_keymap.c
 * installs a strong keycode_at_encodermap_location() that reads
 * exclusively from EEPROM, bypassing this array at runtime entirely. It is
 * still required to exist (keymap_introspection.c's raw accessor indexes
 * it unconditionally whenever ENCODER_MAP_ENABLE is on) and is what a
 * fresh Vial GUI "factory defaults" view would show. The values that
 * actually take effect are seeded into EEPROM below, in
 * eeconfig_init_user().
 */
#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_L1]   = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_L2]   = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_L3]   = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_L4]   = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_L5]   = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_L6]   = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_L7]   = { ENCODER_CCW_CW(KC_BRID, KC_BRIU) },
};
#endif
