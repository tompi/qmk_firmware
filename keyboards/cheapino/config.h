// Copyright 2023 Thomas Haukland (@tompi)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Force the usage of PIO1 peripheral, by default the WS2812 implementation uses the PIO0 peripheral.
#define WS2812_PIO_USE_PIO1
// Confirmed on actual v2 hardware 2026-11-09: RGB produced visibly wrong
// colours (red/green swapped); GRB is correct for this LED strip.
#define WS2812_BYTE_ORDER WS2812_BYTE_ORDER_GRB

// The encoder is read from the matrix by a custom driver in cheapino.c.
#define NUM_ENCODERS 1

// Home row mods resolve by the opposite-hands rule: a tap-hold key settles as
// TAPPED when the next key is on the same hand.  Handedness map and the
// same-hand exceptions are in chordal_hold.c.
#define CHORDAL_HOLD

// Baseline tapping term.  Set here rather than in keyboard.json because the
// schema key name for TAPPING_TERM has moved between QMK versions, and
// get_tapping_term() in the keymap uses this value as its default branch.
#define TAPPING_TERM 200

// Decided 2026-10-09: disabled. Plain 200ms for every key -- same as the
// vial keymap already had unconditionally -- works well enough; the
// opposite-hand-only guarantee below wasn't worth its mouse+modifier cost.
// Re-enable by uncommenting the next line if that changes again.
// #define HOME_ROW_MOD_OPPOSITE_HAND_ONLY

// A tap-hold key on a chordal_hold_layout 'L'/'R' position only resolves to
// its modifier/layer function via a chordal-hold interrupt -- i.e. an
// opposite-hand key (or one of the two [D]/[K] same-hand exceptions in
// chordal_hold.c) -- as long as that interrupt arrives within this window.
// Chordal hold's own interrupt-based decision is unaffected by the window's
// size: it always resolves immediately, same-hand-forces-tap either way.
// What the window's size actually trades off is the timeout fallback, for
// when nothing interrupts at all:
//   - Wider (e.g. 5000ms) protects even a hesitant same-hand pause from
//     misfiring as a modifier, but makes a solo hold (e.g. [Ctrl] alone,
//     then reaching for an external mouse) impractical -- nothing short of
//     holding for the full window ever commits it, and a mouse click/scroll
//     is invisible to QMK's matrix, so it can never serve as the interrupt
//     either.
//   - 500ms keeps same-hand rolls safe at any realistic typing pace (real
//     hesitation rarely exceeds it) while letting a deliberate half-second
//     hold-then-mouse gesture still commit via the plain timeout.
// Used by musclememory/keymap.c's get_tapping_term(). Kept at keyboard level
// rather than duplicated, for whenever a second keymap can use it too.
//
// NOT applied to the vial keymap: quantum/vial.c defines its own strong,
// unconditional get_tapping_term() whenever VIAL_ENABLE=yes (it backs
// Vial-GUI-configurable per-tap-dance timing), which collides with any
// keymap-level definition at link time. Unlike the QMK_SETTINGS/
// get_chordal_hold() collision this isn't gated by a build flag, so it can't
// be sidestepped without patching vial-qmk itself. Until that's worth doing,
// the vial keymap keeps the plain flat TAPPING_TERM for these keys.
#define HOME_ROW_MOD_TERM 500

// Force these values regardless of what the JSON or other files say
#ifdef MOUSEKEY_ENABLE
    #undef MOUSEKEY_INTERVAL
    #define MOUSEKEY_INTERVAL 16

    #undef MOUSEKEY_INITIAL_SPEED
    #define MOUSEKEY_INITIAL_SPEED 2

    #undef MOUSEKEY_MOVE_DELTA
    #define MOUSEKEY_MOVE_DELTA 1  // Set to 1 for absolute precision testing

    #undef MOUSEKEY_MAX_SPEED
    #define MOUSEKEY_MAX_SPEED 14

    #undef MOUSEKEY_TIME_TO_MAX
    #define MOUSEKEY_TIME_TO_MAX 100
#endif
