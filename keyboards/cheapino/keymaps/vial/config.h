/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#define VIAL_KEYBOARD_UID {0x23, 0x5D, 0x98, 0xDE, 0xFB, 0x35, 0xDC, 0x47}

#define VIAL_COMBO_ENABLE
#define VIAL_TAP_DANCE_ENABLE

#define VIAL_UNLOCK_COMBO_ROWS { 4, 0 }
#define VIAL_UNLOCK_COMBO_COLS { 10, 4 }

// Vial tap dance and combo support
#define VIAL_TAP_DANCE_ENTRIES 10
#define VIAL_COMBO_ENTRIES 12

// Twelve layers.  Do NOT reduce this: the published .vil contains 12 layers
// and Vial refuses to import a .vil with more layers than the firmware has.
#define DYNAMIC_KEYMAP_LAYER_COUNT 12
