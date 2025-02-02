/* Aluno: Daniel de Lima Franceschetti */

#include <stdio.h>
#include "pico/stdlib.h"
#include "ws2812.pio.h"

/* BitDogLab built-ins */
#define PIN_BUTTON_A 5
#define PIN_BUTTON_B 6
#define PIN_RED   13
#include "bdl_ws2812.h" /* código para a matriz de LED 5x5 */

#define LENGTH(arr) ((int)(sizeof(arr) / sizeof(*(arr))))

#define SLEEP_PER_LOOP_MS 200 /* para LED vermelho piscar ~5 vezes por segundo */

/* atrasos para o "debouncing" */
#define BUTTON_FALL_DELAY_MS 50
#define BUTTON_RISE_DELAY_MS 50

/* agrupamento para tratar do pressionamento de botões */
typedef struct {
	bool pressed;
	uint32_t last_fall;
	uint32_t last_rise;
	void (*if_pressed)(void);
} Button;

/* dados dos dígitos hexadecimais 5x5 */
static const uint32_t digits[] = {
	[0x0] = 0x0e8d62e, /* 0b0111010001101011000101110 */
	[0x1] = 0x046109f, /* 0b0010001100001000010011111 */
	[0x2] = 0x1e09b1f, /* 0b1111000001001101100011111 */
	[0x3] = 0x1f1383e, /* 0b1111100010011100000111110 */
	[0x4] = 0x0654be2, /* 0b0011001010100101111100010 */
	[0x5] = 0x1f8783e, /* 0b1111110000111100000111110 */
	[0x6] = 0x0e87a2e, /* 0b0111010000111101000101110 */
	[0x7] = 0x1f10884, /* 0b1111100010000100010000100 */
	[0x8] = 0x0e8ba2e, /* 0b0111010001011101000101110 */
	[0x9] = 0x0e8bc3e, /* 0b0111010001011110000111110 */
	[0xa] = 0x0e8fe31, /* 0b0111010001111111000110001 */
	[0xb] = 0x1e8fa3e, /* 0b1111010001111101000111110 */
	[0xc] = 0x0f8420f, /* 0b0111110000100001000001111 */
	[0xd] = 0x1e8c63e, /* 0b1111010001100011000111110 */
	[0xe] = 0x0f8720f, /* 0b0111110000111001000001111 */
	[0xf] = 0x0f87e10, /* 0b0111110000111111000010000 */
};

static volatile Button buttons[PIN_BUTTON_B+1];

/* o número a ser {incre,decre}mentado pelos botões e ser exibido na matriz de LED */
static volatile unsigned number = 0;

/* desenha o número na matriz de LED */
static void
draw_number(void)
{
	uint32_t d;

	d = digits[number];
	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j)
			if (d >> 5*(4-i)+(4-j) & 1)
				ws2812_put_px(j,4-i, 0,0,0x07);
			else
				ws2812_put_px(j,4-i, 0,0,0);

	ws2812_blit();
}

/* botão A incrementa número */
static void
pressed_a(void)
{
	number = (number+1) & 0xf;
	draw_number();
}

/* botão B decrementa número */
static void
pressed_b(void)
{
	number = (number-1) & 0xf;
	draw_number();
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

/* inicializa um botão usando o resistor pull-up interno */
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
	stdio_init_all();
	ws2812_init();

	gpio_init(PIN_RED);
	gpio_set_dir(PIN_RED, GPIO_OUT);

	button_init(PIN_BUTTON_A, pressed_a);
	button_init(PIN_BUTTON_B, pressed_b);

	draw_number();
	for (;;) {
		/* LED vermelho */
		gpio_put(PIN_RED, 1);
		sleep_ms(SLEEP_PER_LOOP_MS*.25f);
		gpio_put(PIN_RED, 0);
		sleep_ms(SLEEP_PER_LOOP_MS*.75f);
	}
}

/* vim:set spelllang=en,pt_br: */
