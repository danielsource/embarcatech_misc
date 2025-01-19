/* Aluno: Daniel de Lima Franceschetti */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define LENGTH(arr) (sizeof(arr) / sizeof(*(arr)))

enum led_color {
	RED   = 0,
	GREEN = 1,
	BLUE  = 2
};

const uint8_t pin_buzzer = 17;
const uint8_t pin_leds[] = {28, 27, 26};
const uint8_t pin_cols[] = {5, 4, 3, 2};
const uint8_t pin_rows[] = {9, 8, 7, 6};

const char key_map[] = {
	'1', '2', '3', 'A',
	'4', '5', '6', 'B',
	'7', '8', '9', 'C',
	'*', '0', '#', 'D'
};

char
get_key(void)
{
	for (int i = 0; i < LENGTH(pin_rows); ++i) {
		gpio_put(pin_rows[i], 1);
		for (int j = 0; j < LENGTH(pin_cols); ++j) {
			if (gpio_get(pin_cols[j])) {
				sleep_ms(20);
				if (!gpio_get(pin_cols[j]))
					continue;
				gpio_put(pin_rows[i], 0);
				return key_map[i*LENGTH(pin_cols) + j];
			}
		}
		gpio_put(pin_rows[i], 0);
	}

	return '\0';
}

int
main(void)
{
	uint count = 0;
	char pressed, last_pressed = '\0';

	uint buz_slice;
	bool buzzer_on;
	pwm_config config;

	stdio_init_all();

	gpio_set_function(pin_buzzer, GPIO_FUNC_PWM);
	buz_slice = pwm_gpio_to_slice_num(pin_buzzer);
	config = pwm_get_default_config();
	pwm_init(buz_slice, &config, true);
	buzzer_on = 0;

	for (int i = 0; i < LENGTH(pin_leds); ++i) {
		gpio_init(pin_leds[i]);
		gpio_set_dir(pin_leds[i], GPIO_OUT);
		gpio_put(pin_leds[i], 0);
	}

	for (int i = 0; i < LENGTH(pin_cols); ++i) {
		gpio_init(pin_cols[i]);
		gpio_set_dir(pin_cols[i], GPIO_IN);
	}

	for (int i = 0; i < LENGTH(pin_rows); ++i) {
		gpio_init(pin_rows[i]);
		gpio_set_dir(pin_rows[i], GPIO_OUT);
	}

	for (;;) {
		pressed = get_key();
		if (pressed && pressed != last_pressed) {
			printf("#%03u pressed: %c\n", ++count, pressed);

			switch (pressed) {
			case 'A':
				gpio_put(pin_leds[RED], !gpio_get(pin_leds[RED]));
				break;
			case 'B':
				gpio_put(pin_leds[GREEN], !gpio_get(pin_leds[GREEN]));
				break;
			case 'C':
				gpio_put(pin_leds[BLUE], !gpio_get(pin_leds[BLUE]));
				break;
			case 'D':
				if (buzzer_on) {
					puts("*buzzer off*");
					pwm_set_gpio_level(pin_buzzer, 0);
				} else {
					puts("*buzzing*");
					pwm_set_clkdiv(buz_slice, 8.f);
					pwm_set_gpio_level(pin_buzzer, 32768u);
				}
				buzzer_on = !buzzer_on;
				break;
			}
		}

		last_pressed = pressed;
		sleep_ms(50);
	}
}
