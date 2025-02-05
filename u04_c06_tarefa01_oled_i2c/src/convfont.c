/* auxiliary code, usage: convfont > font.inc */

#include <stdio.h>
#include <stdint.h>

#define LENGTH(arr) (sizeof(arr)/sizeof((arr)[0]))

#define WIDTH 6

uint8_t font[] = {
0b001000,
0b010100,
0b100010,
0b100010,
0b111110,
0b100010,
0b100010,
0b000000,

0b111100,
0b100010,
0b100010,
0b111100,
0b100010,
0b100010,
0b111100,
0b000000,

0b000000,
0b000000,
0b000000,
0b000000,
0b000000,
0b000000,
0b000000,
0b000000,
};

void
print_bits(uint8_t byte)
{
	uint8_t n = 8;
	while (n) {
		if (byte>>--n & 1) {
			fputs("\033[47;30m", stdout);
			putchar('1');
			fputs("\033[0m", stdout);
		}
		else {
			putchar('0');
		}
	}
}

void
convchar(uint8_t dst[WIDTH], uint8_t src[8])
{
	int x, y;

	for (x = 0; x < WIDTH; ++x)
		dst[x] = 0;
	for (y = 0; y < 8; ++y) {
		for (x = 0; x < WIDTH; ++x) {
			dst[x] |= (src[y]>>(WIDTH-1 - x)&1) << y;
		}
	}
}

int
main(void)
{
	int i, j;
	uint8_t conv[WIDTH];

	for (i = 0; i < LENGTH(font); i += 8) {
		convchar(conv, font+i);
		for (j = 0; j < WIDTH; ++j) {
			print_bits(conv[j]);
			putchar('\n');
		}
		putchar('\n');
	}
	return 0;
}
