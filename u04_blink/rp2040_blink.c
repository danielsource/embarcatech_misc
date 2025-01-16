#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"

#ifdef CYW43_WL_GPIO_LED_PIN
#	define PICO_WIFI
#	define PIN_LED CYW43_WL_GPIO_LED_PIN
#	include "pico/cyw43_arch.h"
#else
#	define PIN_LED 25
#endif

int
led_init(void)
#ifdef PICO_WIFI
{
	return cyw43_arch_init();
}
#else
{
	gpio_init(PIN_LED);
	gpio_set_dir(PIN_LED, GPIO_OUT);
	return PICO_OK;
}
#endif

void
led_set(bool on)
#ifdef PICO_WIFI
{
	cyw43_arch_gpio_put(PIN_LED, on);
	puts(on ? "On" : "Off");
}
#else
{
	gpio_put(PIN_LED, on);
	puts(on ? "On" : "Off");
}
#endif

int
main(void)
{
	int ret;

	bi_decl(bi_program_description("Blink!"));
	bi_decl(bi_1pin_with_name(PIN_LED, "On-board LED"));

	stdio_init_all();
	ret = led_init();

	hard_assert(ret == PICO_OK);

	for (;;) {
		led_set(0);
		sleep_ms(500);
		led_set(1);
		sleep_ms(1000);
	}

	return 0;
}
