#include "matrix-encoder.h"
#include "matrix.h"
#include "quantum.h"

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
