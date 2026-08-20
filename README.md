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

| Task | How |
| --- | --- |
| Choose a preset | CMake Tools status bar, along the bottom edge |
| Build | **Ctrl/Cmd+Shift+B** (target `app`) |
| Flash | *Tasks: Run Task → flash* |
| Debug | **F5**, launch config matching your preset |
| Peripheral registers | **XPERIPHERALS** in the debug sidebar |
| `printf` output | *View: Show Serial Monitor*, `usbmodem` port, 115200 |

For details, see [docs/vscode.md](docs/vscode.md).

## Where to go from here

- `src/main.c` is yours. Board devices (`LED0_RED`, `BUTTON_S0`, …) are
  named in the board's `board.h`; components like the OLED come in via
  `target_link_libraries(app PRIVATE … stefi::oled)`.
- A board with a different pinout — a breadboard, your own baseboard — is
  added in this project without forking stefi-lib: see
  [docs/custom-board.md](docs/custom-board.md).
- stefi-lib is pinned to a release tag in `CMakeLists.txt`
  (`GIT_TAG v0.1.0`) — bump it there to upgrade.
- To hack on stefi-lib itself alongside your app, point the build at a
  local checkout:
  `cmake --preset stefi-debug -DFETCHCONTENT_SOURCE_DIR_STEFI=/path/to/stefi-lib`
- `board_init()` does **not** switch the MCU to 80 MHz; call
  `system_init()` if you need full speed. `printf`/UART works either way.
