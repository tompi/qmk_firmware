# Cheapino Configuration

This firmware implements the [Muscle memory friendly home row mods layout](https://blog.getreu.net/20250826-muscle-memory-friendly-home-row-mods/). 

## Compiling and Flashing

### Using the QMK/QMK repository

1.  **Clone and Setup:**
    ```bash
    git clone https://github.com/qmk/qmk_firmware.git
    cd qmk-qmk
    git submodule update --init --recursive
    ```

2.  **Copy Keyboard Folder:**
    ```bash
    cp -r ../../keyboards/cheapino qmk-qmk/keyboards/
    ```

3.  **Compile:**
    ```bash
    qmk compile -kb cheapino -km musclememory 
    ```

4.  **Flash to Cheapino (RP2040):**
    * **Method 1 (Physical Button):** Hold the right button on the RP2040 controller while 
      plugging in the USB cable.
    *   **Method 2 (Keycode):** Tap the **FN** key (outer left thumb), hold **/** to access Layer 7, then press the **Q key** (`QK_BOOT`).
    * Drag and drop the `.uf2` file onto the `RPI-RP2` drive.


### Using the Vial/QMK repository

Alternatively, you can use the Vial/QMK repository to compile and flash.

1.  **Clone and Setup:**
    ```bash
    git clone https://github.com/vial-kb/vial-qmk
    cd vial-qmk
    git submodule update --init --recursive
    ```

2.  **Copy Keyboard Folder:**
    ```bash
    cp -r ../../keyboards/cheapino vial-qmk/keyboards/
    ```

3.  **Compile:**
    ```bash
    make cheapino/v2:musclememory
    ```


## Clearing EEPROM

Clearing the EEPROM resets all persistent settings (RGB configurations, Vial support
(if compiled in, i.e. keymaps, tap dances, and combos) to the firmware defaults.

In this firmware, `QK_CLEAR_EEPROM` is hard-mapped to the **W key on Layer 7** (left ring, top row) to allow a reset without the GUI.

1.  **Accessing Layer 7:**
    *   Tap the **FN** key (outer left thumb), then hold **/** (bottom-right key, right half).
2.  **Triggering the Clear EEPROM:**
    *   While on Layer 7, press the **W key**.
3.  **Verification:**
    The device will not reboot automatically — press the controller reboot button or disconnect power to apply the reset. The RGB underglow returning to **bright red** confirms the Clear EEPROM was successful.


## RGB Lighting & Layer Feedback

The underglow changes color based on the active layer:

| Layer | Color | Purpose |
|-------|-------|-------|
| Base (_L0) | Custom | Main layer (restores the last saved color) |
| L1 | (inherit) | Reserved / Unused |
| L2 | Red | Numpad Layer |
| L3 | Cyan | Numbers & Navigation |
| L4 | Lavender | Function keys (F1-F24) |
| L5 | Amber | Navigation & Editing |
| L6 | Mint | Mouse control |
| L7 | Pink | RGB & Media control |

If wished for, the led can be switched off for all layers(`RGB_TOG`, layer 7),
or for the base layer only (`RGB_VAD`, layer 7).


## Troubleshooting

* **Firmware too large:** Activate `LTO_ENABLE = yes` in the `rules.mk`.


## Credits

* **Original musclememory keymap:** Custom home-row-mods layout Jens Getreu.
* **Cheapino keyboard:** Thomas Haukland (@tompi).
