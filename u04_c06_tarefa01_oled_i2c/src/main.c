/* Aluno: Daniel de Lima Franceschetti */

#include <stdio.h>
#include "pico/stdlib.h"

/* BitDogLab built-ins */
#define PIN_BUTTON_A 5
#define PIN_BUTTON_B 6
#define PIN_GREEN 11
#define PIN_BLUE 12
#include "bdl_ws2812.h"  /* código para a matriz de LED 5x5 no GPIO7 */
#include "bdl_ssd1306.h" /* código para o display OLED no GPIO14 e GPIO15 */

#define BUTTON_FALL_DELAY_MS 50
#define BUTTON_RISE_DELAY_MS 50

typedef struct {
	bool pressed;
	uint32_t last_fall;
	uint32_t last_rise;
	void (*if_pressed)(void);
} Button;

static volatile Button buttons[PIN_BUTTON_B+1];

static void
led_show_states(void)
{
	static char str[6] = "G0 B0";
	str[1] = gpio_get(PIN_GREEN)? '1' : '0';
	str[4] = gpio_get(PIN_BLUE)? '1' : '0';
	ssd1306_put_info(str);
	ssd1306_update();
	puts(str);
}

static void
led_green_toggle(void)
{
	gpio_put(PIN_GREEN, !gpio_get(PIN_GREEN));
	led_show_states();
}

static void
led_blue_toggle(void)
{
	gpio_put(PIN_BLUE, !gpio_get(PIN_BLUE));
	led_show_states();
}

static void
button_gpio_callback(uint gpio, uint32_t events)
{
	uint32_t now;

	now = time_us_32();

	if (events & GPIO_IRQ_EDGE_FALL) {
		if (!buttons[gpio].pressed
				&& now-buttons[gpio].last_rise
					>= BUTTON_RISE_DELAY_MS*1000ul) {
			buttons[gpio].pressed = true;
			buttons[gpio].last_fall = now;
			buttons[gpio].if_pressed();
		}
	} else {
		if (buttons[gpio].pressed
				&& now-buttons[gpio].last_fall
				>= BUTTON_FALL_DELAY_MS*1000ul) {
			buttons[gpio].pressed = false;
			buttons[gpio].last_rise = now;
		}
	}
}

static void
button_init(uint gpio, void (*if_pressed)(void))
{
	buttons[gpio].if_pressed = if_pressed;
	gpio_init(gpio);
	gpio_set_dir(gpio, GPIO_IN);
	gpio_pull_up(gpio);
	gpio_set_irq_enabled_with_callback(gpio,
			GPIO_IRQ_EDGE_FALL|GPIO_IRQ_EDGE_RISE,
			true,
			&button_gpio_callback);
}

int
main(void)
{
	int ch;

	stdio_init_all();
	ws2812_init();
	ssd1306_init();
	gpio_init(PIN_GREEN); gpio_set_dir(PIN_GREEN, GPIO_OUT);
	gpio_init(PIN_BLUE); gpio_set_dir(PIN_BLUE, GPIO_OUT);
	button_init(PIN_BUTTON_A, led_green_toggle);
	button_init(PIN_BUTTON_B, led_blue_toggle);

	ssd1306_put_str(
			"ABCDEFGHIJKLMNOPQRSTU"
			"VWXYZabcdefghijklmnop"
			"qrstuvwxyz0123456789 "
			"The quick brown fox  "
			"jumps over the lazy  "
			"dog");
	ssd1306_update();
	for (;;) {
		ch = getchar();

		if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == ' ') {
			ssd1306_put_char(ch);
			ssd1306_update();
			ws2812_fill(0x0);
			ws2812_update();
		} else if (ch >= '0' && ch <= '9') {
			ssd1306_put_char(ch);
			ssd1306_update();
			ws2812_put_digit(ch, 0x0, 0x00000600);
			ws2812_update();
		} else if (ch == '\r') {
			ssd1306_clr_chars();
			ssd1306_update();
			ws2812_fill(0x0);
			ws2812_update();
		} else if (ch == '\b') {
			ssd1306_del_char();
			ssd1306_update();
			ws2812_fill(0x0);
			ws2812_update();
		}
	}
}
