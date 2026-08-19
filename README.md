# stefi-quickstart

A ready-to-build application template for
[stefi-lib](https://github.com/THN-mInfC/stefi-lib), the bare-metal
STM32G4/L4 framework used in the TH Nürnberg Embedded Lab. Clone, build,
flash — you get a blinking LED and `printf` over the ST-Link virtual COM
port, then edit `src/main.c`.

## Prerequisites

- `arm-none-eabi-gcc` (with newlib-nano)
- CMake ≥ 3.24
- OpenOCD (for flashing via the on-board ST-Link)

## Build and flash

```sh
git clone https://github.com/kross11480/stefi-quickstart
cd stefi-quickstart
cmake --preset stefi-debug              # fetches stefi-lib automatically
cmake --build --preset stefi-debug
cmake --build --preset stefi-debug --target flash
```

The Nucleo LED blinks; `printf` output appears on the ST-Link virtual COM
port at 115200 baud.

## Starting your own project from this template

Clone into a directory named after your project:

```sh
git clone https://github.com/kross11480/stefi-quickstart my-project
cd my-project
git remote remove origin      # detach from the template's repository
```

Or, without git: download the archive (GitHub → Code → Download ZIP),
unzip it, and rename the folder:

```sh
unzip stefi-quickstart-main.zip
mv stefi-quickstart-main my-project
cd my-project
git init                      # start fresh history (the ZIP has none)
```

Then rename the CMake project: in `CMakeLists.txt`, change

```cmake
project(stefi_app C ASM)
```

to your project's name. The binary is called `app` independent of the
project name; change `add_executable(app src/main.c)` and the `flash`
target's `DEPENDS`/`$<TARGET_FILE:…>` references too if you want it
renamed. Delete `build/` if it exists — preset builds configure into
`build/<preset>` and pick up the new name on the next
`cmake --preset <preset>`.

Presets: `stefi-debug` / `stefi-release` (STefi on NUCLEO-L476RG),
`rohmi-debug` (Rohmi on NUCLEO-G431KB) and `custom-g431-debug` (your own
baseboard on NUCLEO-G431KB — see
[docs/custom-board.md](docs/custom-board.md)). CLion and VS Code pick
these up automatically.

## VS Code

Open the folder and accept the recommended extensions (CMake Tools,
C/C++, Cortex-Debug, Serial Monitor — see `.vscode/extensions.json`);
`arm-none-eabi-gcc` and `openocd` must be on your `PATH`.

- Pick a preset in the CMake Tools status bar — the row along the
  **bottom edge of the VS Code window**, showing the configure preset,
  build preset and launch target. Clicking the configure-preset segment
  opens the preset picker, a dropdown list at the top of the window with
  the `displayName` of every preset in `CMakePresets.json`; type to
  filter, Enter to select. The same picker is reachable from the CMake
  sidebar or via *CMake: Select Configure Preset* in the Command Palette.
  The build presets build the `app` target only, so stefi-lib components
  the app doesn't link are never compiled; if you change the target in
  the status bar, keep it on **app**.
- **Ctrl/Cmd+Shift+B** builds. `Tasks: Run Task → flash` programs the
  board.
- **F5** starts a debug session — OpenOCD plus GDB via Cortex-Debug,
  halting at `main`. Pick the launch configuration matching your preset:
  *Debug (NUCLEO-G431KB)* for `rohmi-debug` / `custom-g431-debug`,
  *Debug (NUCLEO-L476RG)* for `stefi-debug` / `stefi-release`.
- The **XPERIPHERALS** view in the debug sidebar is wired up: both launch
  configurations point `svdFile` at the SVD stefi-lib ships
  (`configs/stm32g431.svd`, `configs/STM32L4x6.svd`), located through
  `${command:cmake.buildDirectory}` so it follows the selected preset. No
  SVD to download by hand.
- IntelliSense comes from CMake Tools, so include paths and MCU flags
  follow the selected preset — no `c_cpp_properties.json` to maintain.
- `printf` output arrives on the ST-Link virtual COM port. Install the
  recommended **Serial Monitor** extension
  (`ms-vscode.vscode-serial-monitor`), then *View: Show Serial Monitor*
  from the Command Palette — it opens as a tab in the bottom panel next
  to TERMINAL. Pick the `usbmodem` port, set baud to **115200**, and
  press **Start Monitoring**. Framing (8 data bits / no parity / 1 stop
  bit), the LF line ending for what you send, and per-line timestamps are
  preset in `.vscode/settings.json` — port and baud are chosen in the
  panel and remembered per workspace. Outside VS Code,
  `screen /dev/tty.usbmodem* 115200` works too.

## Adding a custom board

A board with a different pinout — a breadboard, your own baseboard — is
added in this project without forking stefi-lib: see
[docs/custom-board.md](docs/custom-board.md).

## Where to go from here

- `src/main.c` is yours. Board devices (`LED0_RED`, `BUTTON_S0`, …) are
  named in the board's `board.h`; components like the OLED come in via
  `target_link_libraries(app PRIVATE … stefi::oled)`.
- stefi-lib is pinned to a release tag in `CMakeLists.txt`
  (`GIT_TAG v0.1.0`) — bump it there to upgrade.
- To hack on stefi-lib itself alongside your app, point the build at a
  local checkout:
  `cmake --preset stefi-debug -DFETCHCONTENT_SOURCE_DIR_STEFI=/path/to/stefi-lib`
- `board_init()` does **not** switch the MCU to 80 MHz; call
  `system_init()` if you need full speed. `printf`/UART works either way.
