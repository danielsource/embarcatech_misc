/* Aluno: Daniel de Lima Franceschetti */

#define WS2812_PIN 7
#define WS2812_WIDTH 5
#define WS2812_HEIGHT 5

static struct {
	PIO pio;
	uint sm;
	volatile uint32_t pixels[WS2812_HEIGHT][WS2812_WIDTH];
} WS2812;

static inline void ws2812_init(void);
static inline void ws2812_put_px(int x, int y, uint8_t r, uint8_t g, uint8_t b);
static inline void ws2812_blit(void);
static inline void ws2812_clear(void);

void
ws2812_init(void)
{
	uint offset;

	hard_assert(pio_claim_free_sm_and_add_program_for_gpio_range(
				&ws2812_program,
				&WS2812.pio,
				&WS2812.sm,
				&offset,
				WS2812_PIN,
				1,
				true));

	ws2812_program_init(
			WS2812.pio,
			WS2812.sm,
			offset,
			WS2812_PIN);

	ws2812_blit();

	// this will free resources and unload the PIO program:
	// pio_remove_program_and_unclaim_sm(&ws2812_program, pio, sm, offset);
}

void
ws2812_put_px(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
	WS2812.pixels[WS2812_HEIGHT-1 - y][x] =
		(uint32_t)g<<24 |
		(uint32_t)r<<16 |
		(uint32_t)b<<8;
}

void
ws2812_blit(void)
{
	int i, j;

	for (i = WS2812_HEIGHT-1; i >= 0; --i) {
		for (j = WS2812_WIDTH-1; j >= 0; --j) {
			pio_sm_put_blocking(
					WS2812.pio,
					WS2812.sm,
					WS2812.pixels[i][j]);
		}
		if (--i < 0)
			break;
		for (j = 0; j < WS2812_WIDTH; ++j) {
			pio_sm_put_blocking(
					WS2812.pio,
					WS2812.sm,
					WS2812.pixels[i][j]);
		}
	}

	busy_wait_us(500);
}

void
ws2812_clear(void)
{
	int i, j;

	for (i = 0; i < WS2812_HEIGHT; ++i)
		for (j = 0; j < WS2812_WIDTH; ++j)
			WS2812.pixels[i][j] = 0;
}
