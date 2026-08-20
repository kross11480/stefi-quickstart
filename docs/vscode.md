# VS Code

The project ships a complete VS Code workspace in `.vscode/`: recommended
extensions, build and flash tasks, two debug configurations and the
IntelliSense/Serial Monitor settings. Nothing in there is generated, and
nothing has to be regenerated after a `git pull`.

## Setup

Open the folder and accept the recommended extensions (CMake Tools,
C/C++, Cortex-Debug, Serial Monitor — see `.vscode/extensions.json`);
`arm-none-eabi-gcc` and `openocd` must be on your `PATH`.

On macOS, VS Code started from the Dock does not inherit the `PATH` of
your login shell. If CMake Tools reports `spawn cmake ENOENT`, either
start it with `code .` from a terminal, or point it at an absolute path
with `"cmake.cmakePath"` in your **user** settings.

## Selecting a preset

Pick a preset in the CMake Tools status bar — the row along the
**bottom edge of the VS Code window**, showing the configure preset,
build preset and launch target. Clicking the configure-preset segment
opens the preset picker, a dropdown list at the top of the window with
the `displayName` of every preset in `CMakePresets.json`; type to
filter, Enter to select. The same picker is reachable from the CMake
sidebar or via *CMake: Select Configure Preset* in the Command Palette.

The build presets build the `app` target only, so stefi-lib components
the app doesn't link are never compiled; if you change the target in the
status bar, keep it on **app**.

## Building and flashing

**Ctrl/Cmd+Shift+B** builds. `Tasks: Run Task → flash` programs the
board. Both tasks live in `.vscode/tasks.json`; `flash` runs the CMake
`flash` target, which is OpenOCD against the on-board ST-Link.

The CMake Tools **Run (▶) and Debug buttons are hidden on purpose**
(`cmake.options.advanced` in `.vscode/settings.json`): they start the
built target on your PC, and `app` is an ARM Cortex-M binary, so all
they produce is `exec format error`. The board is programmed by the
`flash` task and debugged with F5.

## Debugging

**F5** starts a debug session — OpenOCD plus GDB via Cortex-Debug,
halting at `main`. Pick the launch configuration matching your preset:
*Debug (NUCLEO-G431KB)* for `rohmi-debug` / `custom-g431-debug`,
*Debug (NUCLEO-L476RG)* for `stefi-debug` / `stefi-release`.

The **XPERIPHERALS** view in the debug sidebar is wired up: both launch
configurations point `svdFile` at the SVD stefi-lib ships
(`configs/stm32g431.svd`, `configs/STM32L4x6.svd`), located through
`${command:cmake.buildDirectory}` so it follows the selected preset. No
SVD to download by hand.

The OpenOCD pane is chatty at every launch. `Unable to match requested
speed 2000 kHz, using 1000 kHz` is the ST-Link picking the nearest clock
it supports, and `halted due to debug-request` with `pc` at
`Reset_Handler` is the expected state right after a reset, before
Cortex-Debug runs on to `main`. Neither is an error.

## IntelliSense

IntelliSense comes from CMake Tools, so include paths and MCU flags
follow the selected preset — no `c_cpp_properties.json` to maintain.

If you also have the **STM32Cube for VS Code** pack installed, it
activates in this folder too and logs errors on every reload (its clangd
finds no compilation database, its CMSIS pack sync fails). Those parts
are switched off for this workspace in `.vscode/settings.json`
(`stm32cube-ide-clangd.enable` and the two `stm32cube-ide-core.*`
keys); the pack keeps working normally in CubeIDE projects.

## Serial output

`printf` output arrives on the ST-Link virtual COM port. Install the
recommended **Serial Monitor** extension
(`ms-vscode.vscode-serial-monitor`), then *View: Show Serial Monitor*
from the Command Palette — it opens as a tab in the bottom panel next
to TERMINAL. Pick the `usbmodem` port, set baud to **115200**, and
press **Start Monitoring**. Framing (8 data bits / no parity / 1 stop
bit), the LF line ending for what you send, and per-line timestamps are
preset in `.vscode/settings.json` — port and baud are chosen in the
panel and remembered per workspace. Outside VS Code,
`screen /dev/tty.usbmodem* 115200` works too.
