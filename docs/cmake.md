# The CMake build

Two files drive the build, and they answer different questions.

`CMakeLists.txt` says **what** the program is: `app` is built from
`src/main.c`, it links stefi-lib, and stefi-lib is fetched automatically
at the release tag pinned in `GIT_TAG`.

`CMakePresets.json` says **which variant** to build: which board
(`STEFI_BOARD`, `APP_BOARD`), Debug or Release, and which directory the
result goes in. Each preset configures into its own `build/<preset>`, so
switching boards never mixes two caches. CLion and VS Code read the same
file, so the IDE and the command line build identically.

## The presets

| Preset | Board | Build type |
| --- | --- | --- |
| `stefi-debug` | STefi on NUCLEO-L476RG | Debug |
| `stefi-release` | STefi on NUCLEO-L476RG | Release |
| `rohmi-debug` | Rohmi on NUCLEO-G431KB | Debug |
| `custom-g431-debug` | your own baseboard on NUCLEO-G431KB, see [custom-board.md](custom-board.md) | Debug |

## Commands

| Task | Command |
| --- | --- |
| See the presets | `cmake --list-presets` |
| Configure | `cmake --preset <preset>` |
| Build | `cmake --build --preset <preset>` |
| Flash | `cmake --build --preset <preset> --target flash` |
| `printf` output | `screen /dev/tty.usbmodem* 115200` |

Configuring is only needed once per preset, and again after you edit
`CMakeLists.txt` or `CMakePresets.json`. Building compiles what changed.

To debug without an IDE, run `openocd -f board/st_nucleo_g4.cfg`
(`st_nucleo_l4.cfg` for the L476) in one terminal, and in another
connect with
`arm-none-eabi-gdb build/<preset>/app -ex "target extended-remote :3333"`.

## Renaming the program

The executable is called `app` regardless of what `project()` says. That
is deliberate: the name appears in more places than a beginner wants to
chase, and nothing depends on it being meaningful. Leave it alone unless
you have a reason.

If you do want it renamed, all of these have to change together, or the
build succeeds and flashing fails:

- `CMakeLists.txt`
  - [`add_executable(app src/main.c)`](../CMakeLists.txt#L24)
  - [`target_link_libraries(app …)`](../CMakeLists.txt#L48)
  - the POST_BUILD block, which uses
    [`$<TARGET_FILE:app>`](../CMakeLists.txt#L51-L54) five times to run
    `size` and to write the `.bin` and `.hex` images
  - the `flash` target's
    [`$<TARGET_FILE:app>` and `DEPENDS app`](../CMakeLists.txt#L66-L67)
- `CMakePresets.json`: `"targets": ["app"]` in all four build presets
- `.vscode/tasks.json`: `"targets": ["app"]` in the `build` task

`$<TARGET_FILE:app>` is a generator expression: CMake replaces it with
the full path of the built ELF when it writes the build files, which is
why the flash command and the image conversions keep working no matter
which preset, and therefore which build directory, is active.

## The build directory

Everything CMake produces lives in `build/<preset>`, including the
stefi-lib checkout under `build/<preset>/_deps/stefi-src`. None of it is
in git and none of it is yours to edit: changes there are overwritten on
the next configure. Deleting `build/` is always safe, and re-running
`cmake --preset <preset>` recreates it.
