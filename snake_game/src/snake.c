#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/rand.h"
#include "ws2812.pio.h"

/* BitDogLab built-ins */
#define PIN_BUTTON_A 5
#define PIN_BUTTON_B 6
#include "bdl_ws2812.h"

#define GAME_TICK_MS 1000

#define SNAKE_INITIAL_SIZE 2

#define COLOR_SNAKE           0x03000000
#define COLOR_SNAKE_HEAD      0x21000100
#define COLOR_SNAKE_HURT      0x00000100
#define COLOR_SNAKE_HEAD_HURT 0x00001100
#define COLOR_EMPTY           0x00000000
#define COLOR_APPLE           0x00440000
#define COLOR_YOU_WON         0x32520200
#define COLOR_INVALID         0x01010100

#define BUTTON_FALL_DELAY_MS 50
#define BUTTON_RISE_DELAY_MS 50

enum {TileSnake=-1, TileEmpty=0, TileApple=1};
enum {DirN, DirS, DirE, DirW};
enum {SceneGame, SceneGameOver, SceneGameWon};

typedef struct {
	bool pressed;
	uint32_t last_fall;
	uint32_t last_rise;
	void (*if_pressed)(void);
} Button;

static volatile struct {
	int8_t scene;
	int8_t board[25];
	int8_t head;
	int8_t dir;
	bool already_moved;
} Game;

static volatile Button buttons[PIN_BUTTON_B+1];

static void
game_draw(void)
{
	int8_t i;

	fputs("\033[2J", stdout);
	fflush(stdout);

	if (Game.scene == SceneGameWon) {
		ws2812_fill(COLOR_YOU_WON);
		ws2812_blit();
		puts(":)");
		return;
	}

	for (i = 0; i < 25; ++i) {
		if (i%5 == 0)
			putchar('\n');
		printf("%3d", Game.board[i]);

		if (Game.board[i] <= TileSnake) {
			WS2812.pixels[i] = Game.scene==SceneGameOver
				? COLOR_SNAKE_HURT : COLOR_SNAKE;
		} else if (Game.board[i] == TileEmpty) {
			WS2812.pixels[i] = COLOR_EMPTY;
		} else if (Game.board[i] == TileApple) {
			WS2812.pixels[i] = COLOR_APPLE;
		} else {
			WS2812.pixels[i] = COLOR_INVALID;
		}
	}
	putchar('\n');

	WS2812.pixels[Game.head] = Game.scene==SceneGameOver
		? COLOR_SNAKE_HEAD_HURT : COLOR_SNAKE_HEAD;
	ws2812_blit();
}

static void
game_gen_apple(void)
{
	int8_t i, pos_count = 0;
	int8_t valid_pos[25];

	for (i = 0; i < 25; ++i)
		if (Game.board[i] == TileEmpty)
			valid_pos[pos_count++] = i;

	if (pos_count == 0) {
		Game.scene = SceneGameWon;
		return;
	}

	Game.board[ valid_pos[get_rand_32()%pos_count] ] = TileApple;
}

static bool
game_update(__unused struct repeating_timer *t)
{
	int8_t i, lasthead;

	Game.already_moved = false;

	switch (Game.scene) {
	case SceneGame:     goto scene_game;
	case SceneGameOver: goto scene_game_over;
	case SceneGameWon:  goto scene_game_won;
	}

scene_game:
	lasthead = Game.head;

	switch (Game.dir) {
	case DirN:
		Game.head -= 5;
		if (Game.head < 0)
			Game.head += 25;
		break;
	case DirS:
		Game.head += 5;
		if (Game.head >= 25)
			Game.head -= 25;
		break;
	case DirE:
		if (Game.head%5 == 4)
			Game.head -= 4;
		else
			Game.head += 1;
		break;
	case DirW:
		if (Game.head%5 == 0)
			Game.head += 4;
		else
			Game.head -= 1;
		break;
	}

	if (Game.board[Game.head] <= TileSnake) {
		Game.scene = SceneGameOver;
	} else if (Game.board[Game.head] == TileApple) {
		Game.board[lasthead] -= 1;
		game_gen_apple();
	}

	Game.board[Game.head] = Game.board[lasthead] - 1;
	for (i = 0; i < 25; ++i)
		if (Game.board[i] <= TileSnake)
			Game.board[i] += 1;

scene_game_over:
scene_game_won:
	game_draw();

	return true;
}

static void
game_init(void)
{
	int8_t i;

	Game.scene = SceneGame;
	Game.head = 10;
	Game.dir = DirE;

	for (i = 0; i < 25; ++i)
		Game.board[i] = TileEmpty;
	Game.board[14] = TileApple;
	Game.board[Game.head] = TileSnake*SNAKE_INITIAL_SIZE;

	Game.already_moved = false;

	game_draw();
}

static void
snake_move_left(void)
{
	if (Game.scene == SceneGameOver || Game.scene == SceneGameWon) {
		game_init();
		return;
	} else if (Game.already_moved) {
		return;
	}
	switch (Game.dir) {
	case DirN:
		Game.dir = DirW;
		break;
	case DirS:
		Game.dir = DirE;
		break;
	case DirE:
		Game.dir = DirN;
		break;
	case DirW:
		Game.dir = DirS;
		break;
	}
	Game.already_moved = true;
}

static void
snake_move_right(void)
{
	if (Game.scene == SceneGameOver || Game.scene == SceneGameWon) {
		game_init();
		return;
	} else if (Game.already_moved) {
		return;
	}
	switch (Game.dir) {
	case DirN:
		Game.dir = DirE;
		break;
	case DirS:
		Game.dir = DirW;
		break;
	case DirE:
		Game.dir = DirS;
		break;
	case DirW:
		Game.dir = DirN;
		break;
	}
	Game.already_moved = true;
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
	struct repeating_timer timer;

	stdio_init_all();
	ws2812_init();
	button_init(PIN_BUTTON_A, snake_move_left);
	button_init(PIN_BUTTON_B, snake_move_right);

	game_init();
	add_repeating_timer_ms(GAME_TICK_MS, game_update, NULL, &timer);

	for (;;)
		tight_loop_contents();
}
