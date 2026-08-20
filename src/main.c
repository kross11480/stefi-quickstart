/* stefi-quickstart: blink the Nucleo LED and log over the ST-Link VCP.
 * LED_NUCLEO exists on every stefi board, so this builds for all presets.
 *
 * Init is done by hand instead of board_init()
 * */
#include <stdio.h>

#include "board.h"
#include "led.h"

#include <libstefi/systick.h>
#include <libstefi/uart.h>

void main(void) {
    uart_configure();               /* USART2 -> ST-Link VCP, enables printf */
    systick_init(BOARD_SYSCLK);
    systick_start();
    printf("stefi-quickstart: board up\r\n");

    led_init(LED_NUCLEO, OUTPUT);
    while (1) {
        led_toggle(LED_NUCLEO);
        systick_delay_ms(500);
    }
}
