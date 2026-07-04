# Adding a custom board

Components (`led_*`, `button_*`, …) compile only against stefi-lib's board
interface (`boards/interface/stefi_board.h`); which concrete board they run
on is decided at link time. So a board with a different pinout — a
breadboard, your own baseboard — is added in **this** project, without
forking stefi-lib.

## 1. Create the board implementation

Create `boards/<name>/board.h` and `board.c` — start from
`boards/custom_g431/`, which targets a custom baseboard on the
NUCLEO-G431KB.

- `board.h` names the device instances (`LED0_RED`, …, `NUM_LEDS`) and
  declares the config tables; it also defines `BOARD_SYSCLK`, the clock the
  MCU boots on (16 MHz HSI16 on the G431, 4 MHz MSI on the L476).
- `board.c` holds the config tables with the actual pins, plus
  `board_init()`.

Only define tables for devices the board has — a missing table fails at
link time, which is the intended "this board can't do that" check.

## 2. Add a preset

Add a configure + build preset in `CMakePresets.json` that sets:

- `APP_BOARD` to the directory name (selects the board implementation the
  app links);
- `STEFI_BOARD` to a bundled board with the **same MCU** (`rohmi_g431` for
  a G431, `nucleo_l476` for an L476) — it still selects the compiler
  flags, linker script and OpenOCD flash config, even though none of that
  board's code is linked.

## 3. Build and flash

```sh
cmake --preset custom-g431-debug
cmake --build --preset custom-g431-debug --target flash
```

## Pin pitfalls

- PA2/PA3 carry the ST-Link VCP UART — claim them and `printf` dies.
- PA13/PA14 are SWD — claim them and you lose flashing/debug.
- The Nucleo LED is PB8 on the G431KB, PA5 on the L476RG.
- On a breadboard, wire LEDs through a series resistor to the pin and
  buttons to GND (the config's `PULL_UP` supplies the idle level).
