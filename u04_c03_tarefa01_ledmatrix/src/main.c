/* Aluno: Daniel de Lima Franceschetti */

#include <stdio.h>
#include "pico/stdlib.h"
#include "ws2812.pio.h"

/* BitDogLab built-ins */
#define PIN_BTN_A 5
#define PIN_BTN_B 6
#include "bdl_ws2812.h" /* código para a matrix de LED 5x5 */

#define LENGTH(arr) ((int)(sizeof(arr) / sizeof(*(arr))))
#define MOD(x, y)   (((x)%(y) + (y))%(y))

#define SLEEP_PER_LOOP 50
#define MS_PER_FRAME 250
#define FRAME_SIZE 5

struct rgb {
	uint8_t r, g, b;
};

typedef struct {
	struct rgb palette[3];
	uint8_t frames[30][FRAME_SIZE*FRAME_SIZE];
} Animation;

static const Animation animations[] = {
#include "animations.inc"
};

static int current_animation = 0;
static int current_frame = 0;

static void
animation_draw(Animation anim, int frame)
{
	int i, j;
	struct rgb *rgb;

	for (i = 0; i < FRAME_SIZE; ++i) {
		for (j = 0; j < FRAME_SIZE; ++j) {
			rgb = &anim.palette[ anim.frames[frame][i*FRAME_SIZE + j] ];
			ws2812_put_px(j, FRAME_SIZE-1 - i, rgb->r, rgb->g, rgb->b);
		}
	}

	ws2812_blit();
	ws2812_clear();
}

static bool
is_btn_pressed_once(uint gpio, bool *lastpressed)
{
	bool pressed;

	pressed = !gpio_get(gpio);
	sleep_ms(20);
	if (pressed != !gpio_get(gpio))
		return false;

	if (pressed) {
		if (!*lastpressed) {
			*lastpressed = true;
			return true;
		}
	} else {
		*lastpressed = false;
	}

	return false;
}

int
main(void)
{
	bool lastpressed_a = false;
	bool lastpressed_b = false;

	uint32_t frame_beg = 0;
	uint32_t frame_end = 0;
	uint32_t time_now = 0;

	stdio_init_all();
	ws2812_init();

	gpio_init(PIN_BTN_A);
	gpio_set_dir(PIN_BTN_A, GPIO_IN);
	gpio_pull_up(PIN_BTN_A);

	gpio_init(PIN_BTN_B);
	gpio_set_dir(PIN_BTN_B, GPIO_IN);
	gpio_pull_up(PIN_BTN_B);

	for (;;) {
		if (is_btn_pressed_once(PIN_BTN_A, &lastpressed_a)) {
			current_animation = MOD(current_animation-1, LENGTH(animations));
			current_frame = 0;
		} else if (is_btn_pressed_once(PIN_BTN_B, &lastpressed_b)) {
			current_animation = MOD(current_animation+1, LENGTH(animations));
			current_frame = 0;
		}

		time_now = time_us_32();
		if (frame_end == 0 || time_now - frame_beg >= MS_PER_FRAME*1000ul) {
			frame_beg = time_now;
			frame_end = frame_beg + MS_PER_FRAME*1000ul;
			animation_draw(animations[current_animation], current_frame);
			current_frame = MOD(current_frame+1, LENGTH(animations[0].frames));
		}

		sleep_ms(SLEEP_PER_LOOP);
	}
}
