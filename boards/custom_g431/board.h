#pragma once

#include "stefi_board.h"

/* Boot clock: the G431 resets onto the 16 MHz HSI16. Passed to
 * systick_init(); update the SysTick (and UART) setup if the clock is
 * reconfigured. */
#define BOARD_SYSCLK (16000000U)

/* Device instances — indices into the config tables in board.c.
 * Rename and extend these to match the devices on your baseboard. */
enum {
    LED0_RED = 0,   // external LED on the baseboard (pin in board.c)
    LED_NUCLEO,     // LD2 on the NUCLEO-G431KB (PB8)
    NUM_LEDS
};

enum {
    BUTTON_USER0 = 0,   // external button to GND (pin in board.c)
    NUM_BUTTONS
};

/* Header pins free for applications — fill in board.c to match what the
 * baseboard leaves unclaimed, and keep this count in sync. */
#define NUM_GPIO_PINS 4

extern led_config_t leds[NUM_LEDS];
extern button_config_t buttons[NUM_BUTTONS];
extern hardware_timer_config_t hardware_timers[NUM_RESERVED_TIMER];
extern freepin_config_t freepin[NUM_GPIO_PINS];

void board_init(void);
