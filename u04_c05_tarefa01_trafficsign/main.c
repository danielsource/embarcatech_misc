/* Aluno: Daniel de Lima Franceschetti */

#define PIN_RED   13
#define PIN_GREEN 11

#include <stdio.h>
#include "pico/stdlib.h"

enum { SignRed, SignYellow, SignGreen, SignCount };

static int traffic_sign_state = SignRed;

static bool
repeating_timer_callback(__unused struct repeating_timer *t)
{
	switch (traffic_sign_state) {
	case SignRed:    gpio_put(PIN_GREEN, 1);                       break;
	case SignYellow: gpio_put(PIN_RED, 0);                         break;
	case SignGreen:  gpio_put(PIN_GREEN, 0); gpio_put(PIN_RED, 1); break;
	}
	traffic_sign_state = (traffic_sign_state+1) % SignCount;
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
		printf("Sinal de trânsito: %d\n", traffic_sign_state);
		sleep_ms(1000);
	}
}
