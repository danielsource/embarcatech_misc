#include <stdio.h>
#include "pico/stdlib.h"
#include "ws2812.pio.h"

/* BitDogLab built-ins */
#define PIN_BUTTON_A 5
#define PIN_BUTTON_B 6
#include "bdl_ws2812.h"

#define GAME_TICK_MS 1000

#define BUTTON_FALL_DELAY_MS 50
#define BUTTON_RISE_DELAY_MS 50

enum BoardTile {TILE_SNAKE=-1, TILE_EMPTY=0, TILE_APPLE=2};

typedef struct {
	bool pressed;
	uint32_t last_fall;
	uint32_t last_rise;
	void (*if_pressed)(void);
} Button;

static volatile struct {
	int8_t board[25];
	int8_t head;
	enum {DIR_N, DIR_S, DIR_E, DIR_W} dir;
} Game;

static volatile Button buttons[PIN_BUTTON_B+1];

static bool repeating_timer_callback(__unused struct repeating_timer *t);

static struct repeating_timer timer;

static void
game_draw(void)
{
	int8_t i;

	for (i = 0; i < 25; ++i) {
		if (Game.board[i] <= TILE_SNAKE) {
			WS2812.pixels[i] = 0x22000000;
		} else if (Game.board[i] == TILE_EMPTY) {
			WS2812.pixels[i] = 0x00000000;
		} else if (Game.board[i] == TILE_APPLE) {
			WS2812.pixels[i] = 0x00440000;
		} else {
			WS2812.pixels[i] = 0x01010100;
		}
	}

	ws2812_blit();
}

static void
game_update()
{
	int8_t i, lasthead;

	lasthead = Game.head;

	switch (Game.dir) {
	case DIR_N:
		Game.head -= 5;
		if (Game.head < 0)
			Game.head += 25;
		break;
	case DIR_S:
		Game.head += 5;
		if (Game.head >= 25)
			Game.head -= 25;
		break;
	case DIR_E:
		if (Game.head%5 == 4)
			Game.head -= 4;
		else
			Game.head += 1;
		break;
	case DIR_W:
		if (Game.head%5 == 0)
			Game.head += 4;
		else
			Game.head -= 1;
		break;
	}

	Game.board[Game.head] = Game.board[lasthead] - 1;

	for (i = 0; i < 25; ++i) {
		if (Game.board[i] <= TILE_SNAKE) {
			Game.board[i] += 1;
		}
	}

	game_draw();
}

static bool
repeating_timer_callback(__unused struct repeating_timer *t)
{
	game_update();
	return true;
}

static void
snake_move_left(void)
{
	switch (Game.dir) {
	case DIR_N:
		Game.dir = DIR_W;
		break;
	case DIR_S:
		Game.dir = DIR_E;
		break;
	case DIR_E:
		Game.dir = DIR_N;
		break;
	case DIR_W:
		Game.dir = DIR_S;
		break;
	}
}

static void
snake_move_right(void)
{
	switch (Game.dir) {
	case DIR_N:
		Game.dir = DIR_E;
		break;
	case DIR_S:
		Game.dir = DIR_W;
		break;
	case DIR_E:
		Game.dir = DIR_S;
		break;
	case DIR_W:
		Game.dir = DIR_N;
		break;
	}
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
	add_repeating_timer_ms(GAME_TICK_MS, repeating_timer_callback, NULL, &timer);

	Game.head = 10;
	Game.dir = DIR_E;
	Game.board[14] = TILE_APPLE;
	Game.board[Game.head] = TILE_SNAKE*2;

	game_draw();

	for (;;) {
		tight_loop_contents();
	}
}

/* vim:set spelllang=en,pt_br: */
