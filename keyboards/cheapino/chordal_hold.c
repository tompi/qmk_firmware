// Copyright 2026 Jens Getreu (@getreu)
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Opposite-hands rule ("bilateral combinations") for the muscle memory
// friendly layout.  Lives at keyboard level so that every keymap shares one
// definition.

#include QMK_KEYBOARD_H

#ifdef CHORDAL_HOLD

// Handedness of every physical key, in LAYOUT_split_3x5_3 order:
//
//   'L' / 'R'  takes part in the opposite-hands rule
//   '*'        exempt, the rule never applies to this key
//
// All six thumb keys and the centre encoder button are exempt, and that is
// mandatory rather than cosmetic:
//
//   * LT(_L6, KC_ESC) sits on the LEFT thumb but raises the mouse layer,
//     whose buttons sit on the LEFT hand.
//   * LT(_L5, KC_BSPC) sits on the RIGHT thumb but raises the navigation
//     layer, whose arrows sit on the RIGHT hand.
//   * LGUI_T(KC_SPC) / LGUI_T(KC_ENT) are used with either hand, and a thumb
//     is biomechanically independent of the fingers anyway.
//
// Giving those thumbs a physical hand would make half of the mouse and
// navigation layers unreachable.
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM =
    LAYOUT(
        'L', 'L', 'L', 'L', 'L',   '*',   'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L',          'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L',          'R', 'R', 'R', 'R', 'R',
                  '*', '*', '*',     '*', '*', '*'
    );

// Matrix positions of the home row keys involved in the same-hand
// exceptions, read from the "layouts" block of keyboard.json.
#define POS_D 5, 8
#define POS_F 5, 7
#define POS_K 1, 2
#define POS_J 1, 1

static inline bool is_pos(keypos_t k, uint8_t row, uint8_t col) {
    return k.row == row && k.col == col;
}

// Matched by matrix POSITION, not by keycode: while LT(_L3, KC_D) is still
// undecided the symbol layer is not yet active, so it is not defined whether
// the other key arrives as the base layer LCTL_T(KC_F) or as the _L3
// LCTL_T(KC_TAB).  Positions sidestep that, and also survive a Vial remap of
// whatever sits on those keys.
bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t *tap_hold_record,
                      uint16_t other_keycode, keyrecord_t *other_record) {
    (void)tap_hold_keycode;
    (void)other_keycode;

    // Exception 1: hold [D] (symbol layer), tap [F] -> TAB.
    if (is_pos(tap_hold_record->event.key, POS_D) &&
        is_pos(other_record->event.key, POS_F)) {
        return true;
    }

    // Exception 2, the mirror image: hold [K] (symbol layer), tap [J] -> DOWN.
    if (is_pos(tap_hold_record->event.key, POS_K) &&
        is_pos(other_record->event.key, POS_J)) {
        return true;
    }

    // Everything else follows the opposite-hands rule.
    return get_chordal_hold_default(tap_hold_record, other_record);
}

#endif  // CHORDAL_HOLD
