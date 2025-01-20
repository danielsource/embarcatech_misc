/* Using BitDogLab (https://github.com/BitDogLab/BitDogLab) */

#define PIN_RED   13
#define PIN_GREEN 11
#define PIN_BLUE  12

#include <stdio.h>
#include "pico/stdlib.h"

#define LENGTH(arr) (sizeof(arr) / sizeof(*(arr)))
#define forever for(;;)

static const uint8_t pin_leds[] = {PIN_RED, PIN_GREEN, PIN_BLUE};

int
main(void)
{
	stdio_init_all();

	for (size_t i = 0; i < LENGTH(pin_leds); ++i) {
		gpio_init(pin_leds[i]);
		gpio_set_dir(pin_leds[i], GPIO_OUT);
		gpio_put(pin_leds[i], 0);
	}

	forever {
		for (size_t i = 0; i < LENGTH(pin_leds); ++i) {
			gpio_put(pin_leds[i], 1);
			sleep_ms(1000);
			gpio_put(pin_leds[i], 0);
			sleep_ms(1000);
		}
	}
}
