/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "blink.pio.h"

// For WaveShare Core2350B board's LED pin
#define CORE2350B_LED_PIN 39

#ifndef PIO_BLINK_LED1_GPIO
#define PIO_BLINK_LED1_GPIO CORE2350B_LED_PIN
#endif

void blink_pin_forever(PIO pio, uint sm, uint offset, uint pin, uint freq);

int main()
{
    stdio_init_all();
    sleep_ms(2000); 

    printf("Starting\n");

    setup_default_uart();

    PIO pio[2];
    uint sm[2];
    uint offset[2];

    // Find a free pio and state machine and add the program
    bool rc = pio_claim_free_sm_and_add_program_for_gpio_range(
                &blink_program, &pio[0], &sm[0], &offset[0], 
                PIO_BLINK_LED1_GPIO, 2, true);

    //hard_assert(rc);
    printf("Loaded program at %u on pio %u\n", offset[0], PIO_NUM(pio[0]));

    // Start led1 flashing
    blink_pin_forever(pio[0], sm[0], offset[0], PIO_BLINK_LED1_GPIO, 4);

    // free up pio resources
    pio_sm_unclaim(pio[0], sm[0] + 1);
    pio_remove_program_and_unclaim_sm(&blink_program, pio[0], sm[0], offset[0]);

    // the program exits but the pio keeps running!
    printf("All leds should be flashing\n");
}

void blink_pin_forever(PIO pio, uint sm, uint offset, uint pin, uint freq) 
{
    blink_program_init(pio, sm, offset, pin);
    pio_sm_set_enabled(pio, sm, true);

    printf("Blinking pin %d at %d Hz\n", pin, freq);

    // PIO counter program takes 3 more cycles in total than we pass as
    // input (wait for n + 1; mov; jmp)
    pio->txf[sm] = (clock_get_hz(clk_sys) / (2 * freq)) - 3;
}
