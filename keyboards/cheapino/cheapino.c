// Copyright 2023 Thomas Haukland (@tompi)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "matrix.h"
#include "quantum.h"
#include "print.h"

#include QMK_KEYBOARD_H

// This is to keep state between callbacks, when it is 0 the
// initial RGB flash is finished
uint8_t _hue_countdown = 50;

// These are to keep track of user selected color, so we
// can restore it after RGB flash
uint8_t _hue;
uint8_t _saturation;
uint8_t _value;

// Do a little 2.5 seconds display of the different colors
// Use the deferred executor so the LED flash dance does not
// stop us from using the keyboard.
// https://docs.qmk.fm/#/custom_quantum_functions?id=deferred-executor-registration
uint32_t flash_led(uint32_t next_trigger_time, void *cb_arg) {
    rgblight_sethsv(_hue_countdown * 5, 230, 70);
    _hue_countdown--;
    if (_hue_countdown == 0) {
        // Finished, reset to user chosen led color
        rgblight_sethsv(_hue, _saturation, _value);
        return 0;
    } else {
        return 50;
    }
}

void keyboard_post_init_kb(void) {
    // debug_enable=true;
    // debug_matrix=true;
    // debug_keyboard=true;
    // debug_mouse=true;

    // Store user selected rgb hsv:
    _hue        = rgblight_get_hue();
    _saturation = rgblight_get_sat();
    _value      = rgblight_get_val();

    // Flash a little on start
    defer_exec(50, flash_led, NULL);
}

// Ghosting
//
// The Cheapino uses a "duplex" matrix: every column pin carries two logical
// columns that are scanned in opposite directions (even columns: drive the
// column, read the rows; odd columns: drive the row, read the columns), each
// with its own diode direction. That saves pins, but lets current sneak
// through two held keys in one row when they sit on different column pins
// and scan in opposite directions. A third key in another row, on the same
// pin as one of them, then makes a key that isn't pressed show up.
//
// Example (QWERTY, left half): hold Q and W, press D, and A shows up too.
// See https://github.com/tompi/cheapino/issues/33
//
// QMK's MATRIX_HAS_GHOST can't catch this. It only looks for the classic
// rectangle (two rows sharing two logical columns), which never happens here
// because the sneak path uses both logical columns of one pin. It also
// ignores the whole row while a ghost is possible, which would drop real
// chords such as home row mods.
//
// Rule, per half: with keys (i, a), (i, b) and (j, a ^ 1) held, where a and b
// are on different pins (a / 2 != b / 2) and scan in opposite directions
// (a % 2 != b % 2), a ghost appears at (j, b). Remove it.

#define GHOST_ROWS_PER_HALF 3 // finger rows; the 4th row of each half is not part of this
#define COLS_PER_HALF 6

static void fix_ghosting_half(matrix_row_t matrix[], uint8_t first_row, uint8_t col_shift) {
    uint8_t rows[GHOST_ROWS_PER_HALF];
    uint8_t ghosts[GHOST_ROWS_PER_HALF] = {0};

    for (uint8_t r = 0; r < GHOST_ROWS_PER_HALF; r++) {
        rows[r] = (matrix[first_row + r] >> col_shift) & ((1 << COLS_PER_HALF) - 1);
    }

    for (uint8_t i = 0; i < GHOST_ROWS_PER_HALF; i++) {
        for (uint8_t j = 0; j < GHOST_ROWS_PER_HALF; j++) {
            if (i == j) continue;
            for (uint8_t a = 0; a < COLS_PER_HALF; a++) {
                if (!(rows[i] & (1 << a)) || !(rows[j] & (1 << (a ^ 1)))) continue;
                for (uint8_t b = 0; b < COLS_PER_HALF; b++) {
                    bool other_pin       = (b >> 1) != (a >> 1);
                    bool other_direction = (b & 1) != (a & 1);
                    if (other_pin && other_direction && (rows[i] & (1 << b))) {
                        ghosts[j] |= 1 << b;
                    }
                }
            }
        }
    }

    for (uint8_t r = 0; r < GHOST_ROWS_PER_HALF; r++) {
        matrix[first_row + r] &= ~((matrix_row_t)ghosts[r] << col_shift);
    }
}

void fix_ghosting(matrix_row_t matrix[]) {
    fix_ghosting_half(matrix, 0, 0);             // right half: rows 0-2, columns 0-5
    fix_ghosting_half(matrix, 4, COLS_PER_HALF); // left half: rows 4-6, columns 6-11
}

// Encoder
//
// There aren't enough pins on the RJ45 for dedicated encoder pins, so the
// encoder's A and B contacts sit on matrix intersections. The matrix scan
// hands them over here and clears them, so they never reach the keymap (where
// they could, for example, resolve a tap-hold key early). The encoder press is
// a normal key at [3, 0].

#define ENC_ROW 3
#define ENC_A_BIT ((matrix_row_t)1 << 2)
#define ENC_B_BIT ((matrix_row_t)1 << 4)

static bool enc_a = false;
static bool enc_b = false;

void fix_encoder_action(matrix_row_t current_matrix[]) {
    enc_a = current_matrix[ENC_ROW] & ENC_A_BIT;
    enc_b = current_matrix[ENC_ROW] & ENC_B_BIT;
    current_matrix[ENC_ROW] &= ~(ENC_A_BIT | ENC_B_BIT);
}

void encoder_driver_init(void) {}

void encoder_driver_task(void) {
    static bool both_closed = false;

    if (enc_a && enc_b) {
        both_closed = true;
    } else if (both_closed && (enc_a || enc_b)) {
        // A+B followed by only A means clockwise, only B counter-clockwise
        both_closed = false;
        encoder_queue_event(0, enc_a);
    }
}
