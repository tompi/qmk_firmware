#include QMK_KEYBOARD_H

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
 *
 * TAP DANCE DEFINITION
 *
 */
// - Base Layer: Single Tap = KC_APP | Hold = Activate L2 momentarily | Double Tap = Toggle L2
// - Other Layers: Any interaction = layer_clear() (Reset to Base)
enum { TD_CMENU, TD_KP_PLUS, TD_KP_MINUS, TD_KP_DOT, TD_KP_0 };

static bool kp_0_held = false;
static bool kp_peql_held = false;

void td_kp_0_finished(tap_dance_state_t *state, void *user_data) {
    if (state->pressed) {
        if (state->count == 1) {
            kp_peql_held = true;
            register_code(KC_PEQL);
        } else {
            kp_0_held = true;
            register_code(KC_KP_0);
        }
    } else {
        tap_code(KC_KP_0);
    }
}

void td_kp_0_reset(tap_dance_state_t *state, void *user_data) {
    if (kp_peql_held) {
        unregister_code(KC_PEQL);
        kp_peql_held = false;
    }
    if (kp_0_held) {
        unregister_code(KC_KP_0);
        kp_0_held = false;
    }
}

static bool kp_dot_held = false;
static bool kp_pcmm_held = false;

void td_kp_dot_finished(tap_dance_state_t *state, void *user_data) {
    if (state->pressed) {
        if (state->count == 1) {
            kp_pcmm_held = true;
            register_code(KC_PCMM);
        } else {
            kp_dot_held = true;
            register_code(KC_KP_DOT);
        }
    } else {
        tap_code(KC_KP_DOT);
    }
}

void td_kp_dot_reset(tap_dance_state_t *state, void *user_data) {
    if (kp_pcmm_held) {
        unregister_code(KC_PCMM);
        kp_pcmm_held = false;
    }
    if (kp_dot_held) {
        unregister_code(KC_KP_DOT);
        kp_dot_held = false;
    }
}

static bool kp_minus_held = false;
static bool kp_psls_held = false;

void td_kp_minus_finished(tap_dance_state_t *state, void *user_data) {
    if (state->pressed) {
        if (state->count == 1) {
            kp_psls_held = true;
            register_code(KC_PSLS);
        } else {
            kp_minus_held = true;
            register_code(KC_KP_MINUS);
        }
    } else {
        tap_code(KC_KP_MINUS);
    }
}

void td_kp_minus_reset(tap_dance_state_t *state, void *user_data) {
    if (kp_psls_held) {
        unregister_code(KC_PSLS);
        kp_psls_held = false;
    }
    if (kp_minus_held) {
        unregister_code(KC_KP_MINUS);
        kp_minus_held = false;
    }
}

static bool kp_plus_held = false;
static bool kp_past_held = false;

void td_kp_plus_finished(tap_dance_state_t *state, void *user_data) {
    if (state->pressed) {
        if (state->count == 1) {
            kp_past_held = true;
            register_code(KC_PAST);
        } else {
            kp_plus_held = true;
            register_code(KC_KP_PLUS);
        }
    } else {
        tap_code(KC_KP_PLUS);
    }
}

void td_kp_plus_reset(tap_dance_state_t *state, void *user_data) {
    if (kp_past_held) {
        unregister_code(KC_PAST);
        kp_past_held = false;
    }
    if (kp_plus_held) {
        unregister_code(KC_KP_PLUS);
        kp_plus_held = false;
    }
}

// True while td_cmenu_finished() has switched _L2 on as a momentary layer.
// Tracking this explicitly is necessary because get_highest_layer() reports
// a higher layer when one is active at release time: hold [Compose] for
// momentary numpad, then reach [D]/[K] (reachable -- _L2's left half is
// transparent) to raise _L3, release [Compose] first -- the highest layer
// is now _L3, so a get_highest_layer()==_L2 check never fires and the
// numpad stays on forever.
static bool td_cmenu_momentary_l2 = false;

void td_cmenu_finished(tap_dance_state_t *state, void *user_data) {
    if (!state->pressed && state->count >= 2) {
        // Double tap always activates L2 exclusively, from any layer
        td_cmenu_momentary_l2 = false;
        layer_move(_L2);
    } else if (get_highest_layer(layer_state) == _BASE) {
        if (state->pressed) {
            // Hold = Momentarily activate L2
            layer_on(_L2);
            td_cmenu_momentary_l2 = true;
        } else {
            tap_code16(KC_APP);
        }
    } else {
        // If we are on ANY other layer, reset everything to Base
        layer_clear();
    }
}

void td_cmenu_reset(tap_dance_state_t *state, void *user_data) {
    if (td_cmenu_momentary_l2) {
        layer_off(_L2);
        td_cmenu_momentary_l2 = false;
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_CMENU]    = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_cmenu_finished, td_cmenu_reset),
    [TD_KP_PLUS]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_kp_plus_finished, td_kp_plus_reset),
    [TD_KP_MINUS] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_kp_minus_finished, td_kp_minus_reset),
    [TD_KP_DOT]   = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_kp_dot_finished, td_kp_dot_reset),
    [TD_KP_0]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_kp_0_finished, td_kp_0_reset)
};

// PER-KEY TAPPING TERM
// Dynamically adjusts the tapping term.
// Uses the global TAPPING_TERM from keyboard.json/config.h as the baseline.
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // Relaxed window for the TD_4 tap dance (Menu/Layer 2 Toggle)
        case TD(TD_CMENU):
            return 350;

        // Slightly more buffer for thumb-based layer taps (optional)
        case LT(_L6, KC_ESC):
        case MT(MOD_LGUI, KC_SPC):
        case MT(MOD_LGUI, KC_ENT):
        case LT(_L5, KC_BSPC):
            return 250;

        // Everything else: any tap-hold key whose matrix position carries a
        // hand ('L'/'R') in chordal_hold_layout gets HOME_ROW_MOD_TERM,
        // regardless of which layer redefines it there (e.g. _L3's [F]
        // position is LCTL_T(KC_TAB), _L2's is LALT_T(KC_NUM_LOCK) -- both
        // still that same matrix position).  Checking position instead of
        // keycode means this can't silently miss a layer's own variant.
        // Thumb/center positions ('*') fall through to TAPPING_TERM.
        //
        // Tap dances are excluded even though e.g. the numpad's TD_KP_PLUS/
        // MINUS/DOT/0 sit on 'R' positions: process_tap_dance.c also calls
        // get_tapping_term() to time its own inter-tap window, which has
        // nothing to do with chordal hold's opposite-hands decision. Without
        // this check those four keys would (and did) inherit
        // HOME_ROW_MOD_TERM and turn noticeably sluggish.
        default:
#ifdef HOME_ROW_MOD_OPPOSITE_HAND_ONLY
            if (!IS_QK_TAP_DANCE(keycode) &&
                chordal_hold_handedness(record->event.key) != '*') {
                return HOME_ROW_MOD_TERM;
            }
#endif
            // Fall back to TAPPING_TERM from config.h.  (This used to be a
            // hardcoded 200 under a comment claiming it was a fallback.)
            return TAPPING_TERM;
    }
}


/**
 *
 * POST-PROCESSING: post_process_record_user
 * Logic that runs AFTER a key press is handled.
 * Specifically used to clear layers (return to base) after a Tap-Hold key is tapped.
 *
 */
void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Return to the base layer when the layer-tap thumb key was tapped.
    if (!record->event.pressed && record->tap.count > 0) {
        switch (keycode) {
            case LT(_L6, KC_NO):
                layer_clear(); // Reset to Base Layer
                break;
        }
    }
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
 * KEYMAP DATA
 * The matrix definition for each layer.
 */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_BASE] = LAYOUT(
    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_MUTE, KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
    LSFT_T(KC_A), RALT_T(KC_S), LT(_L3, KC_D), LCTL_T(KC_F), LALT_T(KC_G), LALT_T(KC_H), RCTL_T(KC_J), LT(_L3, KC_K), RALT_T(KC_L), RSFT_T(KC_SCLN),
    KC_Z,    KC_X,    KC_C,    RGUI_T(KC_V), KC_B,    KC_N,    RGUI_T(KC_M), KC_COMM, KC_DOT,  KC_SLSH,
    OSL(_L4), LT(_L6, KC_ESC), MT(MOD_LGUI, KC_SPC), MT(MOD_LGUI, KC_ENT), LT(_L5, KC_BSPC), TD(TD_CMENU)
),

// _L1 is intentionally unused and kept only to preserve layer numbering:
// renumbering would invalidate the published .vil and every Vial screenshot.
[_L1] = LAYOUT(
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, LT(_L6, KC_NO), KC_TRNS,           KC_TRNS, KC_TRNS, KC_TRNS
),

[_L2] = LAYOUT(
    KC_TRNS,        KC_TRNS,        KC_TRNS,         KC_TRNS,        KC_TRNS, KC_TRNS, TD(TD_KP_PLUS),      KC_KP_7,         KC_KP_8,          KC_KP_9,         TD(TD_KP_0),
    KC_TRNS,        KC_TRNS,        KC_TRNS,         KC_TRNS,        KC_TRNS,         LALT_T(KC_NUM_LOCK), RCTL_T(KC_KP_4), LT(_L3, KC_KP_5), RALT_T(KC_KP_6), RSFT_T(KC_PENT),
    KC_TRNS,        KC_TRNS,        KC_TRNS,         KC_TRNS,        KC_TRNS,         TD(TD_KP_MINUS),     RGUI_T(KC_KP_1), KC_KP_2,          KC_KP_3,         TD(TD_KP_DOT),
    KC_TRNS, LT(_L6, KC_NO), KC_TRNS,           KC_TRNS, KC_TRNS, KC_TRNS
),

[_L3] = LAYOUT(
    KC_1,           KC_2,           KC_3,             KC_4,           KC_5,  KC_TRNS,  KC_6,            KC_7,            KC_8,          KC_9,            KC_0,
    LSFT_T(KC_ESC), RALT_T(KC_INS), LT(_L3,KC_DEL),   LCTL_T(KC_TAB), LALT_T(KC_BSPC), LALT_T(KC_LEFT), RCTL_T(KC_DOWN), LT(_L3,KC_UP), RALT_T(KC_RGHT), RSFT_T(KC_ENT),
    KC_GRV,         KC_MINS,        KC_EQL,           RGUI_T(KC_LBRC), KC_RBRC,        KC_BSLS,         RGUI_T(KC_QUOT), KC_COMM,       KC_DOT,          KC_SLSH,
    KC_TRNS, LT(_L6, KC_NO), KC_TRNS,           KC_TRNS, KC_TRNS, KC_TRNS
),

[_L4] = LAYOUT(
    KC_F1,          KC_F2,          KC_F3,   KC_F4,           KC_F5,   QK_REBOOT, KC_F6,          KC_F7,          KC_F8,   KC_F9,          KC_F10,
    LSFT_T(KC_F11), RALT_T(KC_F12), KC_F13,  LCTL_T(KC_F14),  LALT_T(KC_F15),     LALT_T(KC_F16), RCTL_T(KC_F17), KC_F18,  RALT_T(KC_F19), RSFT_T(KC_F20),
    KC_F21,         KC_F22,         KC_F23,  RGUI_T(KC_F24),  KC_NO,              KC_NO,          RGUI_T(KC_NO),  KC_NO,   KC_NO,          MO(_L7),
    KC_TRNS, LT(_L6, KC_NO), KC_TRNS,           KC_TRNS, KC_TRNS, KC_TRNS
),

[_L5] = LAYOUT(
    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,  KC_HOME, KC_INS,  KC_DEL,  KC_END,  KC_BSPC,
    KC_ESC,  KC_INS,  KC_DEL,  KC_TAB,  KC_BSPC,          KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_ENT,
    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,            KC_NO,   KC_PGDN, KC_PGUP, KC_NO,   KC_NO,
    KC_TRNS, LT(_L6, KC_NO), KC_TRNS,           KC_TRNS, KC_TRNS, KC_TRNS
),

[_L6] = LAYOUT(
    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  KC_NO,  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
    MS_BTN5, MS_BTN1, MS_BTN3, MS_BTN2, MS_BTN4,          MS_LEFT, MS_DOWN, MS_UP,   MS_RGHT, MS_BTN1,
    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,            MS_WHLL, MS_WHLD, MS_WHLU, MS_WHLR, KC_NO,
    KC_TRNS, LT(_L6, KC_NO), KC_TRNS,           KC_TRNS, KC_TRNS, KC_TRNS
),

[_L7] = LAYOUT(
    QK_BOOT,   QK_CLEAR_EEPROM, QK_UNDERGLOW_VALUE_DOWN, QK_UNDERGLOW_VALUE_UP, QK_UNDERGLOW_TOGGLE, KC_NO,   KC_NO,   KC_BRID, KC_BRIU, KC_NO, KC_NO,
    QK_REBOOT, QK_UNDERGLOW_SATURATION_DOWN,         QK_UNDERGLOW_HUE_DOWN, QK_UNDERGLOW_HUE_UP, QK_UNDERGLOW_SATURATION_UP,          KC_MUTE, KC_VOLD, KC_VOLU, KC_NO, KC_NO,
    KC_NO,     KC_NO,           KC_NO,   KC_NO,   KC_NO,            KC_MPLY, KC_MPRV, KC_MNXT, KC_NO, KC_NO,
    KC_TRNS, LT(_L6, KC_NO), KC_TRNS,           KC_TRNS, KC_TRNS, KC_TRNS
)
};

/**
 * ENCODER
 * fix_encoder_action() in cheapino.c injects the rotation events; without a
 * consumer they were discarded.  Index 0 is the only encoder.  Volume
 * everywhere, brightness on the system layer.  KC_TRNS on every other layer
 * falls through to the base layer's volume binding.
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

/**
 * COMBOS
 * Combos allow triggering actions by pressing multiple keys simultaneously.
 * - combo1/2: Trigger Layer 7 (System settings).
 * - combo3: Go to Layer 3.
 * - combo4/5: Reboot into Bootloader (Flash mode).
 * - combo_caps: Toggle Caps Lock.
 * - combo_numpad: Go to Layer 2 (numpad).
 */
const uint16_t PROGMEM combo1[] = { RGUI_T(KC_M), MT(MOD_LGUI, KC_ENT), COMBO_END };
const uint16_t PROGMEM combo2[] = { RGUI_T(KC_V), MT(MOD_LGUI, KC_SPC), COMBO_END };
const uint16_t PROGMEM combo3[] = { LT(_L6, KC_ESC), LT(_L5, KC_BSPC), COMBO_END };
const uint16_t PROGMEM combo4[] = { LT(_L6, KC_NO), LT(_L5, KC_BSPC), COMBO_END };
const uint16_t PROGMEM combo_caps[] = { OSL(_L4), TD(TD_CMENU), COMBO_END };
const uint16_t PROGMEM combo_numpad[] = { KC_COMM, KC_DOT, COMBO_END };

combo_t key_combos[] = {
    COMBO(combo1, MO(_L7)),
    COMBO(combo2, MO(_L7)),
    COMBO(combo3, TO(_L3)),
    COMBO(combo4, TO(_L3)),
    COMBO(combo_caps, KC_CAPS),
    COMBO(combo_numpad, TO(_L2))
};
