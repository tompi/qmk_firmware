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
