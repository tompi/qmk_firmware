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

// This is just to be able to declare constants as they appear in the qmk console
#define rev(b) \
            ((b & 1) << 15) | \
            ((b & (1 << 1)) << 13) | \
            ((b & (1 << 2)) << 11) | \
            ((b & (1 << 3)) << 9) | \
            ((b & (1 << 4)) << 7) | \
            ((b & (1 << 5)) << 5) | \
            ((b & (1 << 6)) << 3) | \
            ((b & (1 << 7)) << 1) | \
            ((b & (1 << 8)) >> 1) | \
            ((b & (1 << 9)) >> 3) | \
            ((b & (1 << 10)) >> 5) | \
            ((b & (1 << 11)) >> 7) | \
            ((b & (1 << 12)) >> 9) | \
            ((b & (1 << 13)) >> 11) | \
            ((b & (1 << 14)) >> 13) | \
            b >> 15

/* This is for debugging the matrix rows
void printBits(uint16_t n)
{
    long i;
    for (i = 15; i >= 0; i--) {
        if ((n & (1 << i)) != 0) {
            printf("1");
        }
        else {
            printf("0");
        }
    }
    printf("\n");
}
*/

bool bit_pattern_set(uint16_t number, uint16_t bitPattern) {
    return !(~number & bitPattern);
}

void fix_ghosting_instance(
        matrix_row_t current_matrix[],
        unsigned short row_num_with_possible_error_cause,
        uint16_t possible_error_cause,
        unsigned short row_num_with_possible_error,
        uint16_t possible_error,
        uint16_t error_fix) {
    if (bit_pattern_set(current_matrix[row_num_with_possible_error_cause], possible_error_cause)) {
        if (bit_pattern_set(current_matrix[row_num_with_possible_error], possible_error)) {
            current_matrix[row_num_with_possible_error] = current_matrix[row_num_with_possible_error] ^ error_fix;
        }
    }
}

void fix_ghosting_column(
        matrix_row_t matrix[],
        uint16_t possible_error_cause,
        uint16_t possible_error,
        uint16_t error_fix) {
    // First the right side
    for (short i = 0; i<3; i++) {
        fix_ghosting_instance(matrix, i, possible_error_cause, (i+1)%3, possible_error, error_fix);
        fix_ghosting_instance(matrix, i, possible_error_cause, (i+2)%3, possible_error, error_fix);
    }

    // Then exactly same procedure on the left side
    for (short i = 0; i<3; i++) {
        fix_ghosting_instance(matrix, i+4, possible_error_cause<<6, 4+((i+1)%3), possible_error<<6, error_fix<<6);
        fix_ghosting_instance(matrix, i+4, possible_error_cause<<6, 4+((i+2)%3), possible_error<<6, error_fix<<6);
    }
}

// For QWERTY layout, key combo a+s+e also outputs q. This suppresses the q, and other similar ghosts
// These are observed ghosts(following a pattern). TODO: need to fix this for v3
// Might need to add 2 diodes(one in each direction) for every row, to increase voltage drop.
void fix_ghosting(matrix_row_t matrix[]) {
    fix_ghosting_column(matrix,
                        rev(0B0110000000000000),
                        rev(0B1010000000000000),
                        rev(0B0010000000000000));
    fix_ghosting_column(matrix,
                        rev(0B0110000000000000),
                        rev(0B0101000000000000),
                        rev(0B0100000000000000));

    fix_ghosting_column(matrix,
                        rev(0B0001100000000000),
                        rev(0B0010100000000000),
                        rev(0B0000100000000000));
    fix_ghosting_column(matrix,
                        rev(0B0001100000000000),
                        rev(0B0001010000000000),
                        rev(0B0001000000000000));

    fix_ghosting_column(matrix,
                        rev(0B1000010000000000),
                        rev(0B1000100000000000),
                        rev(0B1000000000000000));
    fix_ghosting_column(matrix,
                        rev(0B1000010000000000),
                        rev(0B0100010000000000),
                        rev(0B0000010000000000));

    fix_ghosting_column(matrix,
                        rev(0B1001000000000000),
                        rev(0B0101000000000000),
                        rev(0B0001000000000000));
    fix_ghosting_column(matrix,
                        rev(0B1001000000000000),
                        rev(0B1010000000000000),
                        rev(0B1000000000000000));

    fix_ghosting_column(matrix,
                        rev(0B0100100000000000),
                        rev(0B0100010000000000),
                        rev(0B0100000000000000));
    fix_ghosting_column(matrix,
                        rev(0B0100100000000000),
                        rev(0B1000100000000000),
                        rev(0B0000100000000000));
}

void encoder_driver_task(void) {
    // This is intentionally left empty to disable the default encoder driver.
    // We inject events manually below.
}

// There aren't enough pins on the RJ45 for dedicated encoder pins. Use matrix
// intersections instead.
void fix_encoder_action(matrix_row_t current_matrix[]) {
    static const int ENC_ROW   = 3;
    static const int ENC_A_COL = 2;
    static const int ENC_B_COL = 4;
    // The button column is unused here and handled through the keymap instead.
    // static const int ENC_BUTTON_COL = 0;
    static const matrix_row_t ENC_A_BIT = (1 << ENC_A_COL);
    static const matrix_row_t ENC_B_BIT = (1 << ENC_B_COL);

    // State machine tracking.
    static bool colABPressed = false;

    // Check which way the encoder is turned:
    matrix_row_t encoder_row = current_matrix[ENC_ROW];
    bool         colA        = encoder_row & ENC_A_BIT;
    bool         colB        = encoder_row & ENC_B_BIT;

    extern bool encoder_queue_event(uint8_t, bool);
    if (colA && colB) {
        colABPressed = true;
    } else if (colA) {
        if (colABPressed) {
            // A+B followed by A means clockwise
            colABPressed = false;
            encoder_queue_event(0, true);
        }
    } else if (colB) {
        if (colABPressed) {
            // A+B followed by B means counter-clockwise
            colABPressed = false;
            encoder_queue_event(0, false);
        }
    }

    // Clear A+B bits, and leave the button bits intact; it will be picked up
    // by normal matrix/keymap processing.
    static const matrix_row_t ROW_MASK = ~(ENC_A_BIT | ENC_B_BIT);
    current_matrix[ENC_ROW]            = encoder_row & ROW_MASK;
}
