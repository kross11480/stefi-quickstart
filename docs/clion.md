# CLion

CLion builds this project from the same `CMakePresets.json` as the
command line and VS Code, so nothing has to be configured twice. Menu
names below are from CLion 2024/2025; older versions put the same things
in slightly different places.

## Opening the project

Open the folder. CLion reads `CMakePresets.json` and offers the configure
presets as CMake profiles; enable the one for your board. If the offer
does not appear, the profiles are in *Settings → Build, Execution,
Deployment → CMake*, and *File → Reload CMake Project* re-reads the file.

Each profile configures into the preset's own `build/<preset>`
directory, so the command line and CLion share one build tree. CLion's
own default profiles would instead use `cmake-build-debug/` and friends,
which are gitignored; with presets enabled you do not need them, and
keeping both around only means building everything twice.

`arm-none-eabi-gcc` and `openocd` must be on the `PATH` that CLion sees.
On macOS this is the reason a build can work in the terminal and fail in
the IDE: an app started from the Dock does not inherit your shell's
`PATH`.

## Building

Select the profile and the `app` target in the run-configuration
dropdown, then build (**Cmd/Ctrl+F9**). The presets restrict the build to
`app`, so stefi-lib components the app does not link are never compiled.

## Flashing

The `flash` target runs OpenOCD against the on-board ST-Link. From
CLion's terminal:

```sh
cmake --build build/<preset> --target flash
```

For a button, add an *OpenOCD Download and Run* configuration (Run → Edit
Configurations → +). It requires the bundled Embedded Development plugin
and OpenOCD's path in *Settings → Build, Execution, Deployment → Embedded
Development*. Point it at:

- **Target** and **Executable**: `app`
- **Board config file**: `st_nucleo_g4.cfg` for the G431 or
  `st_nucleo_l4.cfg` for the L476, either from OpenOCD's own board list
  or from `build/<preset>/_deps/stefi-src/configs/st_nucleo_g4.cfg`

The same configuration programs the board and starts a debug session, so
it replaces both the `flash` target and a separate debug setup.

## Debugging

Debug the OpenOCD configuration above. To get the peripheral register
view, open the **Peripherals** tab in the debugger and load the SVD
stefi-lib ships:

- `build/<preset>/_deps/stefi-src/configs/stm32g431.svd`
- `build/<preset>/_deps/stefi-src/configs/STM32L4x6.svd`

Those paths follow the selected preset, since the stefi-lib checkout
lives inside the preset's build directory.

## Serial output

`printf` goes to the ST-Link virtual COM port at 115200 baud, 8 data
bits, no parity, 1 stop bit. CLion bundles a Serial Monitor tool window;
pick the `usbmodem` port and that baud rate. Outside the IDE,
`screen /dev/tty.usbmodem* 115200` works too.
