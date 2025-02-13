/* Aluno: Daniel de Lima Franceschetti */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

/* BitDogLab built-ins */
#define PIN_BTN_A 5
#define PIN_JS_BTN 22
#define PIN_JS_X 27
#define PIN_JS_Y 26
#define PIN_RED 13
#define PIN_GREEN 11
#define PIN_BLUE 12
#include "bdl_ssd1306.h" /* código para o display OLED no GPIO14 e GPIO15 */

#define PWM_SLICE_RED_BLUE 6
#define PWM_SLICE_GREEN 5

#define ADC_MAX 4095

#define JS_DEAD_ZONE 260

#define BTN_FALL_DELAY_MS 50
#define BTN_RISE_DELAY_MS 50

#define ABS(x)                ((x)<0 ? -(x) : (x))
#define BETWEEN(x, low, high) ((x) >= (low) && (x) <= (high))

typedef struct {
	bool pressed;
	uint32_t last_fall;
	uint32_t last_rise;
	void (*if_pressed)(void);
} Button;

static volatile Button buttons[PIN_JS_BTN+1];

static void
toggle_led_red_blue(void)
{
	static bool on = true;
	if (on) {
		pwm_set_gpio_level(PIN_RED, 0);
		pwm_set_gpio_level(PIN_BLUE, 0);
		pwm_set_enabled(PWM_SLICE_RED_BLUE, false);
	} else {
		pwm_set_enabled(PWM_SLICE_RED_BLUE, true);
	}
	on = !on;
}

static void
toggle_led_green(void)
{
	gpio_put(PIN_GREEN, !gpio_get(PIN_GREEN));
}

static void
button_gpio_callback(uint gpio, uint32_t events)
{
	uint32_t now;

	now = time_us_32();

	if (events & GPIO_IRQ_EDGE_FALL) {
		if (!buttons[gpio].pressed
				&& now-buttons[gpio].last_rise
					>= BTN_RISE_DELAY_MS*1000ul) {
			buttons[gpio].pressed = true;
			buttons[gpio].last_fall = now;
			buttons[gpio].if_pressed();
		}
	} else {
		if (buttons[gpio].pressed
				&& now-buttons[gpio].last_fall
				>= BTN_FALL_DELAY_MS*1000ul) {
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
	int i, j;
	int jx, jy, jx_raw, jy_raw;
	char jstr[SSD1306_WIDTH / SSD1306_FONT_WIDTH /* 21 */ + 1];

	stdio_init_all();
	ssd1306_init();
	gpio_init(PIN_GREEN); gpio_set_dir(PIN_GREEN, GPIO_OUT);
	button_init(PIN_BTN_A, toggle_led_red_blue);
	button_init(PIN_JS_BTN, toggle_led_green);

	adc_init();
	adc_gpio_init(PIN_JS_X);
	adc_gpio_init(PIN_JS_Y);

	gpio_set_function(PIN_RED, GPIO_FUNC_PWM);
	gpio_set_function(PIN_BLUE, GPIO_FUNC_PWM);
	pwm_set_clkdiv(PWM_SLICE_RED_BLUE, 40.0f);
	pwm_set_wrap(PWM_SLICE_RED_BLUE, 100);
	pwm_set_enabled(PWM_SLICE_RED_BLUE, 1);

	for (;;) {
		adc_select_input(PIN_JS_X-26);
		jx_raw = adc_read();
		adc_select_input(PIN_JS_Y-26);
		jy_raw = adc_read();

		if (BETWEEN(jx_raw, ADC_MAX/2 - JS_DEAD_ZONE, ADC_MAX/2 + JS_DEAD_ZONE))
			jx = SSD1306_WIDTH/2;
		else
			jx = jx_raw * SSD1306_WIDTH / ADC_MAX;

		if (BETWEEN(jy_raw, ADC_MAX/2 - JS_DEAD_ZONE, ADC_MAX/2 + JS_DEAD_ZONE))
			jy = SSD1306_HEIGHT/2;
		else
			jy = SSD1306_HEIGHT-1 - (jy_raw * SSD1306_HEIGHT / ADC_MAX);

		snprintf(jstr, sizeof(jstr), "%3d %3d     %4d %4d",
				jx, jy, jx_raw, jy_raw);
		ssd1306_put_info(jstr);

		if (gpio_get(PIN_GREEN)) {
			for (i = 0; i < 4; ++i)
				for (j = 0; j < SSD1306_WIDTH; ++j)
					if (j & 1)
						ssd1306_flip_bit(j, i);
			for (i = SSD1306_HEIGHT-14; i < SSD1306_HEIGHT-12; ++i)
				for (j = 0; j < SSD1306_WIDTH; ++j)
					if (j & 1)
						ssd1306_flip_bit(j, i);
			for (i = 4; i < SSD1306_HEIGHT-14; ++i)
				for (j = 0; j < 4; ++j)
					if (i & 1)
						ssd1306_flip_bit(j, i);
			for (i = 4; i < SSD1306_HEIGHT-14; ++i)
				for (j = SSD1306_WIDTH-4; j < SSD1306_WIDTH; ++j)
					if (i & 1)
						ssd1306_flip_bit(j, i);
		}

		for (i = 0; i < 8; ++i)
			for (j = 0; j < 8; ++j)
				ssd1306_flip_bit(jx-4+j, jy-4+i);

		ssd1306_update();
		ssd1306_clear();

		pwm_set_gpio_level(PIN_RED,  (float)ABS(jx - SSD1306_WIDTH/2) / (SSD1306_WIDTH/2) * 100);
		pwm_set_gpio_level(PIN_BLUE, (float)ABS(jy - SSD1306_HEIGHT/2) / (SSD1306_HEIGHT/2) * 70);

		sleep_ms(33);
	}
}
