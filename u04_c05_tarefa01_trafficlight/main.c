/* Aluno: Daniel de Lima Franceschetti */

#define PIN_RED   13
#define PIN_GREEN 11

#include <stdio.h>
#include "pico/stdlib.h"

enum { LightRed, LightYellow, LightGreen, LightCount };

static volatile int traffic_light_state = LightRed;

static bool
repeating_timer_callback(__unused struct repeating_timer *t)
{
	switch (traffic_light_state) {
	case LightRed:    gpio_put(PIN_GREEN, 1);                       break;
	case LightYellow: gpio_put(PIN_RED, 0);                         break;
	case LightGreen:  gpio_put(PIN_GREEN, 0); gpio_put(PIN_RED, 1); break;
	}
	traffic_light_state = (traffic_light_state+1) % LightCount;
	return true;
}

int
main(void)
{
	struct repeating_timer timer;

	stdio_init_all();

	gpio_init(PIN_RED);
	gpio_set_dir(PIN_RED, GPIO_OUT);
	gpio_init(PIN_GREEN);
	gpio_set_dir(PIN_GREEN, GPIO_OUT);

	gpio_put(PIN_RED, 1);
	add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);

	for (;;) {
		printf("Farol de trânsito: %d\n", traffic_light_state);
		sleep_ms(1000);
	}
}
