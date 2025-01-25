/* Aluno: Daniel de Lima Franceschetti */

#include <stdio.h>
#include "pico/stdlib.h"
#include "ws2812.pio.h"

/* BitDogLab built-ins */
#define PIN_BTN_A 5
#define PIN_BTN_B 6
#include "bdl_ws2812.h" /* código para a matrix de LED 5x5 */

/* Keypad (usado no Wokwi) */
#ifdef KEYPAD
const uint8_t keypad_cols[] = {21, 20, 19, 18};
const uint8_t keypad_rows[] = {28, 27, 26, 22};
#endif /* KEYPAD end */

#define LENGTH(arr) ((int)(sizeof(arr) / sizeof(*(arr))))
#define MOD(x, y)   (((x)%(y) + (y))%(y))

#define SLEEP_PER_LOOP 50
#define MS_PER_FRAME 250

struct rgb {
	uint8_t r, g, b;
};

typedef struct {
	struct rgb palette[3];
	uint8_t frames[30][5*5];
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

	for (i = 0; i < 5; ++i) {
		for (j = 0; j < 5; ++j) {
			rgb = &anim.palette[ anim.frames[frame][i*5 + j] ];
			ws2812_put_px(j, 4-i, rgb->r, rgb->g, rgb->b);
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

	if (pressed && *lastpressed)
		return false;
	*lastpressed = pressed;
	return pressed;
}

#ifdef KEYPAD
static char
keypad_get_key(void)
{
	static const char key_map[] = {
		'1', '2', '3', 'A',
		'4', '5', '6', 'B',
		'7', '8', '9', 'C',
		'*', '0', '#', 'D'
	};

	static char lastkey = '\0';

	int i, j;
	char key = '\0';

	for (i = 0; i < LENGTH(keypad_rows); ++i) {
		gpio_put(keypad_rows[i], 1);
		for (j = 0; j < LENGTH(keypad_cols); ++j) {
			if (gpio_get(keypad_cols[j])) {
				sleep_ms(20);
				if (!gpio_get(keypad_cols[j]))
					continue;
				gpio_put(keypad_rows[i], 0);
				key = key_map[i*LENGTH(keypad_cols) + j];
				goto end;
			}
		}
		gpio_put(keypad_rows[i], 0);
	}

end:
	if (key && key == lastkey)
		return '\0';
	lastkey = key;
	return key;
}
#endif /* KEYPAD end */

int
main(void)
{
	bool lastpressed_a = false;
	bool lastpressed_b = false;

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

#ifdef KEYPAD
	char key;

	for (int i = 0; i < LENGTH(keypad_cols); ++i) {
		gpio_init(keypad_cols[i]);
		gpio_set_dir(keypad_cols[i], GPIO_IN);
	}

	for (int i = 0; i < LENGTH(keypad_rows); ++i) {
		gpio_init(keypad_rows[i]);
		gpio_set_dir(keypad_rows[i], GPIO_OUT);
	}
#endif /* KEYPAD end */

	for (;;) {
		if (is_btn_pressed_once(PIN_BTN_A, &lastpressed_a)) {
			current_animation = MOD(current_animation-1, LENGTH(animations));
			current_frame = 0;
		} else if (is_btn_pressed_once(PIN_BTN_B, &lastpressed_b)) {
			current_animation = MOD(current_animation+1, LENGTH(animations));
			current_frame = 0;
		}

#ifdef KEYPAD
		key = keypad_get_key();
		switch (key) {
		case '1': current_animation = 5; break;
		case '2': current_animation = 6; break;
		case 'A': current_animation = 0; break;
		case 'B': current_animation = 1; break;
		case 'C': current_animation = 2; break;
		case 'D': current_animation = 3; break;
		case '#': current_animation = 4; break;
		default: goto keyskip;
		}
		current_frame = 0;
keyskip:
#endif /* KEYPAD end */

		time_now = time_us_32();
		if (time_now >= frame_end) {
			frame_end = time_now + MS_PER_FRAME*1000ul;
			animation_draw(animations[current_animation], current_frame);
			current_frame = MOD(current_frame+1, LENGTH(animations[0].frames));
		}

		sleep_ms(SLEEP_PER_LOOP);
	}
}
