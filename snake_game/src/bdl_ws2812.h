/* NOTE: this code targets BitDogLab v6.3 using its 5x5 LED matrix via GPIO7;
         based on this example: https://github.com/raspberrypi/pico-examples/tree/master/pio/ws2812

LED logical positions (in this version of bdl_ws2812.h):
  0 1 2 3 4
0 @ @ @ @ @
1 @ @ @ @ @
2 @ @ @ @ @
3 @ @ @ @ @
4 @ @ @ @ @

LED physical positions (alphabetical order):
(end) -> y x w v u
         p q r s t
         o n m l k
         f g h i j
         e d c b a <- (start)

WS2812.pixels color format:
  0xGGRRBB00 (32 bits)
  GG is the green byte (8 bits)
  RR is the red byte   (8 bits)
  BB is the blue byte  (8 bits)
  00 are the unused least significant 8 bits
*/

static struct {
	PIO pio;
	uint sm;
	volatile uint32_t pixels[25];
} WS2812;

static inline void ws2812_init(void);
static inline void ws2812_blit(void);
static inline void ws2812_fill(uint32_t color);

#define ws2812_put_rgb(x,y, r,g,b)\
	WS2812.pixels[y*5 + x] = (uint32_t)g<<24 | (uint32_t)r<<16 | (uint32_t)b<<8

#define ws2812_put_clr(x,y, color)\
	WS2812.pixels[y*5 + x] = color

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

	ws2812_blit();

	// this will free resources and unload the PIO program:
	// pio_remove_program_and_unclaim_sm(&ws2812_program, pio, sm, offset);
}

void
ws2812_blit(void)
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
