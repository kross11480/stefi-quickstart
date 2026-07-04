cmake_minimum_required(VERSION 3.24)

# Cross toolchain must be set before project(). The presets do this; the
# fallback keeps a plain `cmake -B build` working too.
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/toolchain-arm.cmake
            CACHE FILEPATH "CMake toolchain file")
endif()

project(stefi_app C ASM)
set(CMAKE_C_STANDARD 11)

# Fetch stefi-lib at a pinned release. To develop against a local checkout
# instead, configure with -DFETCHCONTENT_SOURCE_DIR_STEFI=/path/to/stefi-lib.
include(FetchContent)
FetchContent_Declare(stefi
        GIT_REPOSITORY https://github.com/THN-mInfC/stefi-lib.git
        GIT_TAG        v0.1.0)
FetchContent_MakeAvailable(stefi)

add_executable(app src/main.c)

# MCU flags and the linker script propagate from the stefi:: targets.
# stefi::board = the board selected with STEFI_BOARD (default nucleo_l476),
# stefi::simpleio = LEDs/buttons/pins/timers. Add stefi::oled etc. as needed.
# Board implementation the app links against: "stefi" = the board bundled
# with stefi-lib (selected by STEFI_BOARD), otherwise the name of a
# subdirectory of this project's boards/ providing board.{h,c} against
# stefi::board_iface. STEFI_BOARD still selects the MCU flags and linker
# script, so custom boards set it to a bundled board with the same chip.
set(APP_BOARD "stefi" CACHE STRING "App board implementation")
if(APP_BOARD STREQUAL "stefi")
    set(app_board_lib stefi::board)
else()
    add_library(app_board STATIC boards/${APP_BOARD}/board.c)
    target_include_directories(app_board PUBLIC boards/${APP_BOARD})
    target_link_libraries(app_board PUBLIC stefi::board_iface)
    # board_init() drives the SimpleIO devices, same as the bundled boards
    target_link_libraries(app_board PRIVATE stefi::simpleio)
    set(app_board_lib app_board)
endif()

# simpleio before board: led/button code pulls its config arrays from
# board.c, so the linker must see the board impl after stefi::simpleio.
target_link_libraries(app PRIVATE stefi::simpleio ${app_board_lib})

# Size report and .bin/.hex images next to the ELF.
add_custom_command(TARGET app POST_BUILD
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:app>
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:app> $<TARGET_FILE:app>.bin
        COMMAND ${CMAKE_OBJCOPY} -O ihex   $<TARGET_FILE:app> $<TARGET_FILE:app>.hex)

# `cmake --build --preset <preset> --target flash` programs the board via
# the on-board ST-Link. stefi-lib only ships a G4 config; for the L476 we
# use the board file bundled with OpenOCD itself.
if(STEFI_BOARD STREQUAL "rohmi_g431")
    set(STEFI_OPENOCD_CFG ${stefi_SOURCE_DIR}/configs/st_nucleo_g4.cfg)
else()
    set(STEFI_OPENOCD_CFG board/st_nucleo_l4.cfg)
endif()
add_custom_target(flash
        COMMAND openocd -f ${STEFI_OPENOCD_CFG}
                -c "program $<TARGET_FILE:app> verify reset exit"
        DEPENDS app
        COMMENT "Flashing app via OpenOCD")
