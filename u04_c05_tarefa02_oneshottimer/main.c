/* Aluno: Daniel de Lima Franceschetti */

#define PIN_RED    13
#define PIN_GREEN  11
#define PIN_BLUE   12
#define PIN_BUTTON  5

#include <stdio.h>
#include "pico/stdlib.h"

static volatile int led_state;

int64_t turn_off_callback(__unused alarm_id_t id, __unused void *user_data) {
	switch (--led_state) {
	case 2:
		gpio_put(PIN_BLUE, 0);
		puts("amarelo");
		return 3000000;
	case 1:
		gpio_put(PIN_GREEN, 0);
		puts("vermelho");
		return 3000000;
	}

	gpio_put(PIN_RED, 0);
	puts("apagado");
	return 0;
}

int
main(void)
{
	stdio_init_all();

	gpio_init(PIN_RED);
	gpio_set_dir(PIN_RED, GPIO_OUT);
	gpio_init(PIN_GREEN);
	gpio_set_dir(PIN_GREEN, GPIO_OUT);
	gpio_init(PIN_BLUE);
	gpio_set_dir(PIN_BLUE, GPIO_OUT);

	gpio_init(PIN_BUTTON);
	gpio_set_dir(PIN_BUTTON, GPIO_IN);
	gpio_pull_up(PIN_BUTTON);

	for (;;) {
		if (gpio_get(PIN_BUTTON)) {
			sleep_ms(50);
			continue;
		}

		led_state = 3;

		gpio_put(PIN_RED, 1);
		gpio_put(PIN_GREEN, 1);
		gpio_put(PIN_BLUE, 1);
		puts("branco");

		add_alarm_in_ms(3000, turn_off_callback, NULL, false);

		while (led_state > 0)
			tight_loop_contents();
	}
}
