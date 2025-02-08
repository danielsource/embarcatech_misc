#include "pico/stdlib.h"
#include "ws2812.pio.h"

#include "bdl_ws2812.h"

static const uint32_t ws2812_digits[] = {
	[0] = 0x0e8d62e, /* 0b0111010001101011000101110 */
	[1] = 0x046109f, /* 0b0010001100001000010011111 */
	[2] = 0x1e09b1f, /* 0b1111000001001101100011111 */
	[3] = 0x1f1383e, /* 0b1111100010011100000111110 */
	[4] = 0x0654be2, /* 0b0011001010100101111100010 */
	[5] = 0x1f8783e, /* 0b1111110000111100000111110 */
	[6] = 0x0e87a2e, /* 0b0111010000111101000101110 */
	[7] = 0x1f10884, /* 0b1111100010000100010000100 */
	[8] = 0x0e8ba2e, /* 0b0111010001011101000101110 */
	[9] = 0x0e8bc3e, /* 0b0111010001011110000111110 */
};

static struct {
	PIO pio;
	uint sm;
	volatile uint32_t pixels[25];
} WS2812;

void
ws2812_init(void)
{
	uint offset;

	hard_assert(pio_claim_free_sm_and_add_program_for_gpio_range(
				&ws2812_program,
				&WS2812.pio,
				&WS2812.sm,
				&offset,
				7,
				1,
				true));

	ws2812_program_init(
			WS2812.pio,
			WS2812.sm,
			offset,
			7);

	ws2812_update();

	/* this will free resources and unload the PIO program:
	pio_remove_program_and_unclaim_sm(&ws2812_program, pio, sm, offset); */
}

void
ws2812_update(void)
{
	int x;

	for (x = 4; x >= 0; --x)
		pio_sm_put_blocking(
				WS2812.pio,
				WS2812.sm,
				WS2812.pixels[20 + x]);

	for (x = 0; x < 5; ++x)
		pio_sm_put_blocking(
				WS2812.pio,
				WS2812.sm,
				WS2812.pixels[15 + x]);

	for (x = 4; x >= 0; --x)
		pio_sm_put_blocking(
				WS2812.pio,
				WS2812.sm,
				WS2812.pixels[10 + x]);

	for (x = 0; x < 5; ++x)
		pio_sm_put_blocking(
				WS2812.pio,
				WS2812.sm,
				WS2812.pixels[5 + x]);

	for (x = 4; x >= 0; --x)
		pio_sm_put_blocking(
				WS2812.pio,
				WS2812.sm,
				WS2812.pixels[x]);

	busy_wait_us(500);
}

void
ws2812_fill(uint32_t color)
{
	WS2812.pixels[0]  = color;
	WS2812.pixels[1]  = color;
	WS2812.pixels[2]  = color;
	WS2812.pixels[3]  = color;
	WS2812.pixels[4]  = color;
	WS2812.pixels[5]  = color;
	WS2812.pixels[6]  = color;
	WS2812.pixels[7]  = color;
	WS2812.pixels[8]  = color;
	WS2812.pixels[9]  = color;
	WS2812.pixels[10] = color;
	WS2812.pixels[11] = color;
	WS2812.pixels[12] = color;
	WS2812.pixels[13] = color;
	WS2812.pixels[14] = color;
	WS2812.pixels[15] = color;
	WS2812.pixels[16] = color;
	WS2812.pixels[17] = color;
	WS2812.pixels[18] = color;
	WS2812.pixels[19] = color;
	WS2812.pixels[20] = color;
	WS2812.pixels[21] = color;
	WS2812.pixels[22] = color;
	WS2812.pixels[23] = color;
	WS2812.pixels[24] = color;
}

void
ws2812_put_px(uint8_t x, uint8_t y, uint32_t color)
{
	WS2812.pixels[y*5 + x] = color;
}

void
ws2812_put_digit(char digit, uint32_t bg, uint32_t fg)
{
	int i, j;
	uint32_t d;

	d = ws2812_digits[digit-'0'];
	for (i = 0; i < 5; ++i)
		for (j = 0; j < 5; ++j)
			if (d >> 5*(4-i)+(4-j) & 1)
				ws2812_put_px(j, i, fg);
			else
				ws2812_put_px(j, i, bg);
}
