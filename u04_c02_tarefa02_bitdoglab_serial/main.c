#define PIN_RED 13
#define PIN_GREEN 11
#define PIN_BLUE 12
#define PIN_BUZZER 21

#define LENGTH(arr) (sizeof(arr) / sizeof(*(arr)))

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h" /* para buzzer */

typedef void (func_ptr)(void);

static const char pin_leds[] = {PIN_RED, PIN_GREEN, PIN_BLUE};

void turn_off_all(void)
{
	for (int i = 0; i < LENGTH(pin_leds); ++i) {
		gpio_put(pin_leds[i], 0);
	}
}

void turn_on_red(void)   { turn_off_all(); gpio_put(PIN_RED, 1); }
void turn_on_green(void) { turn_off_all(); gpio_put(PIN_GREEN, 1); }
void turn_on_blue(void)  { turn_off_all(); gpio_put(PIN_BLUE, 1); }

void
turn_on_white(void)
{
	for (int i = 0; i < LENGTH(pin_leds); ++i) {
		gpio_put(pin_leds[i], 1);
	}
}

void
turn_on_buzzer(void)
{
	pwm_set_gpio_level(PIN_BUZZER, 32768u);
	sleep_ms(2000);
	pwm_set_gpio_level(PIN_BUZZER, 0);
}

static func_ptr *const functions['~' + 1] = {
	['1'] = turn_on_red,
	['2'] = turn_on_green,
	['3'] = turn_on_blue,
	['4'] = turn_on_white,
	['5'] = turn_off_all,
	['6'] = turn_on_buzzer,
};

int
main(void)
{
	uint buz_slice;
	pwm_config config;

	stdio_init_all();

	for (int i = 0; i < LENGTH(pin_leds); ++i) {
		gpio_init(pin_leds[i]);
		gpio_set_dir(pin_leds[i], GPIO_OUT);
		gpio_put(pin_leds[i], 0);
	}

	gpio_set_function(PIN_BUZZER, GPIO_FUNC_PWM);
	buz_slice = pwm_gpio_to_slice_num(PIN_BUZZER);
	config = pwm_get_default_config();
	pwm_init(buz_slice, &config, true);
	pwm_set_clkdiv(buz_slice, 8.f);

	for (;;) {
		char c;

		c = getchar();
		if (c < ' ' || c > '~')
			continue;
		func_ptr *func = functions[c];
		if (func)
			func();
	}

	return 0;
}
