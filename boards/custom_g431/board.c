#include "board.h"

#include "button.h"
#include "led.h"

#include <libstefi/systick.h>
#include <libstefi/uart.h>

/*
 * Custom baseboard on a NUCLEO-G431KB (Nucleo-32 header).
 *
 * The pin assignments below are placeholders — change them to match the
 * baseboard wiring. Pins to keep clear of on the G431KB:
 *   PA2/PA3    USART2 — printf over the ST-Link VCP
 *   PA13/PA14  SWD — flashing and debugging
 *   PB8        LD2 (kept here as LED_NUCLEO)
 */

led_config_t leds[NUM_LEDS] = {
    [LED0_RED]   = {A0, LOW},          // TODO: baseboard red LED pin
    [LED_NUCLEO] = {B8, LOW},
};

button_config_t buttons[NUM_BUTTONS] = {
    [BUTTON_USER0] = {B4, PULL_UP},    // TODO: baseboard button pin (to GND)
};

_Static_assert(NUM_BUTTONS <= STEFI_MAX_BUTTONS,
        "button.c sizes its per-button state by STEFI_MAX_BUTTONS");

hardware_timer_config_t hardware_timers[NUM_RESERVED_TIMER] = {
    [DEBOUNCE_TIMER] = {TIMER6, 4000, 10},
};

/* Pins the baseboard leaves free for applications, with their timer/ADC
 * routes (see the G431 datasheet AF table). TODO: fill in the real list
 * and keep NUM_GPIO_PINS in board.h in sync. */
freepin_config_t freepin[NUM_GPIO_PINS] = {
    {A5,  TIMER2,        1, NULL, 0},
    {A6,  TIMER3,        1, NULL, 0},
    {A7,  TIMER17,       1, NULL, 0},
    {B5,  TIMER3,        2, NULL, 0},
};

void board_init(void) {
    //UART (USART2/PA2/PA3 — ST-Link VCP)
    uart_configure();

    //initialize all leds, default, output not PWM
    for(uint32_t i = 0; i < NUM_LEDS; i++) {
        led_init(i, OUTPUT);
    }

    //initialize all buttons
    for(uint32_t i = 0; i < NUM_BUTTONS; i++) {
        button_init(i);
    }

    //initialize default timers (button debouncing)
    for(uint32_t i = 0; i < NUM_RESERVED_TIMER; i++) {
        timer_init(hardware_timers[i].timer);
        timer_set_period(hardware_timers[i].timer, hardware_timers[i].prescaler, hardware_timers[i].period);
    }

    //initialize systick timer
    systick_init(BOARD_SYSCLK);
    systick_start();
}
