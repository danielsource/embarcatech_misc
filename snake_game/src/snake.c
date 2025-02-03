#include <stdio.h>
#include "pico/stdlib.h"
#include "ws2812.pio.h"

/* BitDogLab built-ins */
#define PIN_BUTTON_A 5
#define PIN_BUTTON_B 6
#include "bdl_ws2812.h"

#define BUTTON_FALL_DELAY_MS 50
#define BUTTON_RISE_DELAY_MS 50

#define LENGTH(arr) ((int)(sizeof(arr) / sizeof(*(arr))))

enum BoardTile {TILE_EMPTY, TILE_APPLE, TILE_SNAKE};

typedef struct {
	bool pressed;
	uint32_t last_fall;
	uint32_t last_rise;
	void (*if_pressed)(void);
} Button;

static volatile struct {
	uint8_t board[25];
	uint8_t head;
	enum {DIR_N, DIR_E, DIR_S, DIR_W} dir;
} Game;

static volatile Button buttons[PIN_BUTTON_B+1];

static void
snake_move_left(void)
{
}

static void
snake_move_right(void)
{
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
	stdio_init_all();
	ws2812_init();

	button_init(PIN_BUTTON_A, snake_move_left);
	button_init(PIN_BUTTON_B, snake_move_right);

	ws2812_fill(0);
	for (;;) {
		ws2812_blit();
		sleep_ms(3000);
	}
}

/* vim:set spelllang=en,pt_br: */
