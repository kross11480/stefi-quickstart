# stefi-quickstart

A ready-to-build application template for
[stefi-lib](https://github.com/THN-mInfC/stefi-lib), the bare-metal
STM32G4/L4 framework used in the TH Nürnberg Embedded Lab. Clone, build,
flash. The main.c blinks an LED and `printf` over the ST-Link virtual COM
port.

## Prerequisites

- `arm-none-eabi-gcc` (with newlib-nano)
- CMake ≥ 3.24
- OpenOCD (for flashing via the on-board ST-Link)

## Build and flash

The following presets (options) are for the different boards
* `stefi-debug` / `stefi-release` (STefi on NUCLEO-L476RG)
* `rohmi-debug`(Rohmi on NUCLEO-G431KB) 
* `custom-g431-debug` (your own baseboard on NUCLEO-G431KB, see [docs/custom-board.md](docs/custom-board.md)).

Whichever you pick, the result is the same: the Nucleo LED blinks and `printf` output appears on the ST-Link virtual COM port at 115200 baud. One can use command line, vscode, or clion for programming. 

### Command line

```sh
git clone https://github.com/kross11480/stefi-quickstart
cd stefi-quickstart
cmake --preset stefi-debug              # fetches stefi-lib automatically
cmake --build --preset stefi-debug
cmake --build --preset stefi-debug --target flash
```

Every command, and GDB without an IDE: [docs/cmake.md](docs/cmake.md).

### VS Code

**Ctrl/Cmd+Shift+P** → *Git: Clone* fetches the repository, no terminal
needed. Accept the recommended extensions, pick a preset in the status
bar, then **Ctrl/Cmd+Shift+B** to build, *Tasks: Run Task → flash* to
program the board and **F5** to debug: [docs/vscode.md](docs/vscode.md).

### CLion

Open the folder and enable the presets when CLion offers them as CMake
profiles. Build the `app` target, program the board with the `flash`
target, and debug with an OpenOCD configuration:
[docs/clion.md](docs/clion.md).

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

Then give the project your own name: in `CMakeLists.txt`, change

```cmake
project(stefi_app C ASM)
```

to `project(my_project C ASM)`. That is the only line you have to touch,
and the next build picks it up on its own.

The program itself is always called `app`, whatever the project is
named. That is the name the presets build, the name the debugger loads
and the name that gets flashed, so leave it as it is. If you do want it
renamed, [docs/cmake.md](docs/cmake.md) lists every place that has to
change.

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
