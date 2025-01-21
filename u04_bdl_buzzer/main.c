/* Using BitDogLab board (https://github.com/BitDogLab/BitDogLab) */

#pragma GCC diagnostic warning "-Wpedantic"

/* BitDogLab built-ins */
#define PIN_RED    13
#define PIN_GREEN  11
#define PIN_BLUE   12
#define PIN_BUZZER 21

#include <stdio.h>
#include "pico/stdlib.h"

#define LENGTH(arr) (sizeof(arr) / sizeof(*(arr)))

#define forever for(;;)

/* Note frequencies in Hertz */
#define NOTE_B_5 988
#define NOTE_AS5 932
#define NOTE_A_5 880
#define NOTE_GS5 831
#define NOTE_G_5 784
#define NOTE_FS5 740
#define NOTE_F_5 698
#define NOTE_E_5 659
#define NOTE_DS5 622
#define NOTE_D_5 587
#define NOTE_CS5 554
#define NOTE_C_5 523
#define NOTE_B_4 494
#define NOTE_AS4 466
#define NOTE_A_4 440
#define NOTE_GS4 415
#define NOTE_G_4 392
#define NOTE_FS4 370
#define NOTE_F_4 349
#define NOTE_E_4 330
#define NOTE_DS4 311
#define NOTE_D_4 294
#define NOTE_CS4 277
#define NOTE_C_4 262 /* Middle C */
#define NOTE_B_3 247
#define NOTE_AS3 233
#define NOTE_A_3 220
#define NOTE_GS3 208
#define NOTE_G_3 196
#define NOTE_FS3 185
#define NOTE_F_3 175
#define NOTE_E_3 165
#define NOTE_DS3 156
#define NOTE_D_3 147
#define NOTE_CS3 139
#define NOTE_C_3 131
#define NOTE_B_2 123
#define NOTE_AS2 116
#define NOTE_A_2 110
#define NOTE_GS2 104
#define NOTE_G_2  98
#define NOTE_FS2  92
#define NOTE_F_2  87
#define NOTE_E_2  82
#define NOTE_DS2  78
#define NOTE_D_2  73
#define NOTE_CS2  69
#define NOTE_C_2  65
#define NOTE_B_1  62
#define NOTE_AS1  58
#define NOTE_A_1  55

static const uint8_t pin_leds[] = {PIN_RED, PIN_GREEN, PIN_BLUE};

void
tone(uint gpio, uint32_t freq_hz, uint32_t duration_ms)
{
	if (freq_hz == 0)
		return;

	uint32_t half_period_us = 500000ul/freq_hz;
	uint32_t cycle = freq_hz*duration_ms / 1000ul;

	uint led_pin = PIN_GREEN;

	if (freq_hz < 440)
		led_pin = PIN_BLUE;
	else if (freq_hz > 650)
		led_pin = PIN_RED;
	gpio_put(led_pin, 1);

	for (uint32_t i = 0; i < cycle; ++i) {
		gpio_put(gpio, 1);
		sleep_us(half_period_us);
		gpio_put(gpio, 0);
		sleep_us(half_period_us);
	}

	gpio_put(led_pin, 0);
}

int
main(void)
{
	stdio_init_all();

	for (uint8_t i = 0; i < LENGTH(pin_leds); ++i) {
		gpio_init(pin_leds[i]);
		gpio_set_dir(pin_leds[i], GPIO_OUT);
		gpio_put(pin_leds[i], 0);
	}

	gpio_init(PIN_BUZZER);
	gpio_set_dir(PIN_BUZZER, GPIO_OUT);
	gpio_put(PIN_BUZZER, 0);

#	define BPM 76
#	define BEAT_FREQ (60000ul/BPM)
#	define play(note, beats) tone(PIN_BUZZER, note, beats*BEAT_FREQ)
#	define pause(beats) sleep_ms(beats*BEAT_FREQ)
#	define repeat(n) for(size_t i_ = 0; i_ < n; ++i_)

	/* Adapted excerpt from "Subwoofer Lullaby" (Minecraft), by C418 */
	bar01_to_07:
	repeat(3) {
		play(NOTE_C_4, 1);
		play(NOTE_B_4, 1);
		play(NOTE_G_4, 1);
		play(NOTE_B_4, 1);
	}

	bar08:
	play(NOTE_C_4, 1);
	play(NOTE_B_4, 1);
	play(NOTE_G_4, 1);
	play(NOTE_B_4, 0.85f);
	pause         (0.15f);

	bar09:
	play(NOTE_B_4, 0.5f);
	play(NOTE_C_5, 0.5f);
	play(NOTE_D_5, 0.5f);
	play(NOTE_E_5, 1);
	play(NOTE_D_5, 1);
	play(NOTE_B_4, 0.5f);

	bar10:
	play(NOTE_C_5, 1.5f);
	play(NOTE_G_4, 1.5f);
	play(NOTE_B_4, 0.85f);
	pause         (0.15f);

	bar11:
	play(NOTE_B_4, 0.5f);
	play(NOTE_E_5, 0.5f);
	play(NOTE_D_5, 0.5f);
	play(NOTE_E_5, 1);
	play(NOTE_G_5, 1);
	play(NOTE_E_5, 0.5f);

	bar012:
	play(NOTE_E_5, 1);
	play(NOTE_D_5, 2);
	play(NOTE_B_4, 0.85f);
	pause         (0.15f);

	bar13:
	play(NOTE_F_5, 0.5f);
	play(NOTE_E_5, 1);
	play(NOTE_C_5, 1);
	play(NOTE_A_4, 0.5f);
	play(NOTE_C_5, 1);

	bar14:
	play(NOTE_B_4, 0.5f);
	play(NOTE_G_4, 1);
	play(NOTE_E_5, 1);
	play(NOTE_D_5, 0.5f);
	play(NOTE_E_4, 0.5f);
	play(NOTE_F_4, 0.35f);
	pause         (0.15f);

	bar15:
	play(NOTE_F_5, 0.5f);
	play(NOTE_E_5, 1);
	play(NOTE_C_5, 1);
	play(NOTE_A_4, 0.5f);
	play(NOTE_A_5, 1);

	bar16:
	play(NOTE_G_5, 1);
	play(NOTE_E_5, 0.5f);
	play(NOTE_D_5, 1);
	play(NOTE_B_4, 0.5f);
	play(NOTE_G_4, 1);

	bar17_to_20:
	repeat(4) {
		play(NOTE_C_4, 1);
		play(NOTE_G_5, 1);
		play(NOTE_E_5, 1);
		play(NOTE_G_5, 1);
	}

	bar21:
	play(NOTE_G_5, 1);
	play(NOTE_C_4, 1);
	play(NOTE_C_3, 2);

	forever { sleep_ms(10000); }
}

/* vim: set cino+=L0: */
