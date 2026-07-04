/* stefi-quickstart: blink the Nucleo LED and log over the ST-Link VCP.
 * LED_NUCLEO exists on every stefi board, so this builds for all presets.
 *
 * Init is done by hand instead of board_init(): board_init() assumes the
 * full STefi baseboard and hangs forever probing the SSD1306 display over
 * I2C when run on a bare Nucleo. */
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

    led_init(LED0_RED, OUTPUT);
    while (1) {
        led_toggle(LED0_RED);
        systick_delay_ms(500);
    }
}
