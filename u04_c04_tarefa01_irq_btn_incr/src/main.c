/* Aluno: Daniel de Lima Franceschetti */

#include <stdio.h>
#include "pico/stdlib.h"
#include "ws2812.pio.h"

/* BitDogLab built-ins */
#define PIN_BTN_A 5
#define PIN_BTN_B 6
#define PIN_RED   13
#include "bdl_ws2812.h" /* código para a matrix de LED 5x5 */

#define LENGTH(arr) ((int)(sizeof(arr) / sizeof(*(arr))))

#define SLEEP_PER_LOOP_MS     200 /* para LED vermelho piscar ~5 vezes por segundo */
#define BTN_DEBOUNCE_DELAY_MS 200 /* tempo necessário para registrar um aperto de botão */

/* agrupamento para tratar do pressionamento de botões */
typedef struct {
	uint gpio;
	bool last_state;
	bool checked_state;
	uint32_t check_delay;
} Button;

/* dados dos dígitos 5x5 */
static const uint32_t digits[] = {
[0] = 0b0111010001101011000101110, /* TODO: trocar para hex */
[1] = 0b0010001100001000010011111,
[2] = 0b1111000001001101100011111,
[3] = 0b1111100010011100000111110,
[4] = 0b0011001010100101111100010,
[5] = 0b1111110000111100000111110,
[6] = 0b0111010000111101000101110,
[7] = 0b1111100010000100010000100,
[8] = 0b0111010001011101000101110,
[9] = 0b0111010001011110000111110,
};

/* o número a ser {incre,decre}mentado pelos botões e ser exibido na matrix de LED */
static volatile int number = 0;

/* desenha o número na matrix de LED */
static void
draw_number(void)
{
	uint32_t d;

	d = digits[number];
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			if (d >> 5*(4-i)+(4-j) & 1) {
				ws2812_put_px(j,4-i, 0,0,0x07);
			} else {
				ws2812_put_px(j,4-i, 0,0,0);
			}
		}
	}

	ws2812_blit();
	ws2812_clear();
}

/* inicializa um botão usando o resistor pull-up interno */
static Button
btn_init(uint gpio)
{
	Button btn = {.gpio = gpio};
	gpio_init(gpio);
	gpio_set_dir(gpio, GPIO_IN);
	gpio_pull_up(gpio);
	return btn;
}

/* retorna se botão foi pressionado alguma vez desde o último pressionamento */
static bool
is_btn_pressed_once(Button *btn)
{
	bool state;

	/* caso ainda não tenha pressionado: */
	if (btn->check_delay == 0) {
		state = !gpio_get(btn->gpio);
		if (state != btn->last_state) {
			btn->check_delay = time_us_32() + BTN_DEBOUNCE_DELAY_MS*1000ul;
			btn->checked_state = state;
		}
		return false;
	}
	/* caso tenha tenha pressionado porém ainda é cedo: */
	else if (time_us_32() < btn->check_delay) {
		return false;
	}
	btn->check_delay = 0;

	/* finalmente, verifique se o aperto do botão é válido: */
	state = !gpio_get(btn->gpio);
	if (state && (!btn->checked_state || btn->last_state))
		return false;

	btn->last_state = state;
	return state;
}

int
main(void)
{
	Button btn_a;
	Button btn_b;

	stdio_init_all();
	ws2812_init();

	gpio_init(PIN_RED);
	gpio_set_dir(PIN_RED, GPIO_OUT);

	btn_a = btn_init(PIN_BTN_A);
	btn_b = btn_init(PIN_BTN_B);

	draw_number();
	for (;;) {
		/* verificação de aperto de botão (TODO: usar interrupções) */
		if (is_btn_pressed_once(&btn_a)) {                               /* botão A incrementa número */
			number = (number==LENGTH(digits)-1) ? 0 : number+1;
			draw_number();
		} else if (is_btn_pressed_once(&btn_b)) {                        /* botão B decrementa número */
			number = (number==0) ? LENGTH(digits)-1 : number-1;
			draw_number();
		}

		/* LED vermelho */
		/* TODO: usar temporizador em vez de sleep_ms */
		gpio_put(PIN_RED, 1);
		sleep_ms(SLEEP_PER_LOOP_MS*.25f);
		gpio_put(PIN_RED, 0);
		sleep_ms(SLEEP_PER_LOOP_MS*.75f);
	}
}

/* vim:set spelllang=en,pt_br: */
