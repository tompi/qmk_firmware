/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

// Short combo window so combo-member keys that are also tap-hold keys
// (esc/backspace, the mouse-layer/nav-layer triggers, now also combo2/
// combo1's chord partners) aren't buffered long enough to make their own
// hold noticeably sluggish. Defaults to TAPPING_TERM (200ms) otherwise.
#define COMBO_TERM 50
