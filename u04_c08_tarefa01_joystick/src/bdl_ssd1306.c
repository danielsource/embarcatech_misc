#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "bdl_ssd1306.h"

#define LENGTH(arr) (sizeof(arr)/sizeof((arr)[0]))

enum {
	Char_A     = 0,
	Char_a     = Char_A + 26,
	Char_0     = Char_a + 26,
	Char_Space = Char_0 + 10,
	CharCount
};

static const uint8_t font[CharCount * SSD1306_FONT_WIDTH] = {
#include "font.inc"
};

static uint8_t ssd1306_buf[SSD1306_BUF_LEN] = {0x40};

static void
send_cmd(uint8_t cmd)
{
	uint8_t buf[2] = {0x80};
	buf[1] = cmd;
	i2c_write_blocking(SSD1306_I2C_PORT, SSD1306_I2C_ADDR, buf, 2, false);
}

static void
send_cmd_list(uint8_t *buf, int num)
{
	int i;
	for (i = 0; i < num; ++i)
		send_cmd(buf[i]);
}

static void
send_buf(uint8_t *buf, int buflen)
{
	i2c_write_blocking(SSD1306_I2C_PORT, SSD1306_I2C_ADDR, buf, buflen, false);
}

static uint8_t
char_idx(char ch)
{
	if (ch >= 'A' && ch <= 'Z') {
		return ch-'A' + Char_A;
	} else if (ch >= 'a' && ch <= 'z') {
		return ch-'a' + Char_a;
	} else if (ch >= '0' && ch <= '9') {
		return ch-'0' + Char_0;
	} else {
		return Char_Space;
	}
}

void
ssd1306_init(void)
{
	/* https://github.com/raspberrypi/pico-examples/tree/master/i2c/ssd1306_i2c */
	static uint8_t cmds[] = {
		SSD1306_SET_DISP,               /* set display off */
		/* memory mapping */
		SSD1306_SET_MEM_MODE,           /* set memory address mode 0 = horizontal, 1 = vertical, 2 = page */
		0x00,                           /* horizontal addressing mode */
		/* resolution and layout */
		SSD1306_SET_DISP_START_LINE,    /* set display start line to 0 */
		SSD1306_SET_SEG_REMAP | 0x01,   /* set segment re-map, column address 127 is mapped to SEG0 */
		SSD1306_SET_MUX_RATIO,          /* set multiplex ratio */
		SSD1306_HEIGHT - 1,             /* display height - 1 */
		SSD1306_SET_COM_OUT_DIR | 0x08, /* set COM (common) output scan direction; scan from bottom up, COM[N-1] to COM0 */
		SSD1306_SET_DISP_OFFSET,        /* set display offset */
		0x00,                           /* no offset */
		SSD1306_SET_COM_PIN_CFG,        /* set COM (common) pins hardware configuration; board specific magic number; */
		SSD1306_COM_PIN_CFG,            /* 0x02 works for 128x32, 0x12 possibly works for 128x64; other options 0x22, 0x32 */
		/* timing and driving scheme */
		SSD1306_SET_DISP_CLK_DIV,       /* set display clock divide ratio */
		0x80,                           /* div ratio of 1, standard freq */
		SSD1306_SET_PRECHARGE,          /* set pre-charge period */
		0xF1,                           /* VCC internally generated on our board */
		SSD1306_SET_VCOM_DESEL,         /* set VCOMH deselect level */
		0x30,                           /* 0.83xVCC */
		/* display */
		SSD1306_SET_CONTRAST,           /* set contrast control */
		0xFF,
		SSD1306_SET_ENTIRE_ON,          /* set entire display on to follow RAM content */
		SSD1306_SET_NORM_DISP,          /* set normal (not inverted) display */
		SSD1306_SET_CHARGE_PUMP,        /* set charge pump */
		0x14,                           /* VCC internally generated on our board */
		SSD1306_SET_SCROLL | 0x00,      /* deactivate horizontal scrolling if set; this is necessary as memory writes will corrupt if scrolling was enabled */
		SSD1306_SET_DISP | 0x01,        /* turn display on */
	};

	i2c_init(SSD1306_I2C_PORT, SSD1306_I2C_CLK*1000);
	gpio_set_function(SSD1306_I2C_SDA, GPIO_FUNC_I2C);
	gpio_set_function(SSD1306_I2C_SCL, GPIO_FUNC_I2C);
	gpio_pull_up(SSD1306_I2C_SDA);
	gpio_pull_up(SSD1306_I2C_SCL);
	send_cmd_list(cmds, LENGTH(cmds));
	ssd1306_update();
}

void
ssd1306_update(void)
{
	static uint8_t cmds[] = {
		SSD1306_SET_COL_ADDR,
		0,
		SSD1306_WIDTH-1,
		SSD1306_SET_PAGE_ADDR,
		0,
		SSD1306_NUM_PAGES-1
	};
	send_cmd_list(cmds, LENGTH(cmds));
	send_buf(ssd1306_buf, SSD1306_BUF_LEN);
}

void
ssd1306_put_info(const char *msg)
{
	uint8_t x = 0, y = SSD1306_NUM_PAGES-1;
	uint8_t ch;
	int i;
	int len = SSD1306_WIDTH / SSD1306_FONT_WIDTH;

	memset(ssd1306_buf+1 + (y-1)*SSD1306_WIDTH, 0x10, SSD1306_WIDTH);
	memset(ssd1306_buf+1 + y*SSD1306_WIDTH, 0x00, SSD1306_WIDTH);

	while (*msg != '\0' && len--) {
		ch = char_idx(*msg++);
		for (i = 0; i < SSD1306_FONT_WIDTH; ++i)
			ssd1306_buf[1 + y*SSD1306_WIDTH + x + i] =
				font[ch*SSD1306_FONT_WIDTH + i];
		x += SSD1306_FONT_WIDTH;
	}
}

void
ssd1306_flip_bit(uint8_t x, uint8_t y)
{
	uint8_t page;

	if (x > SSD1306_WIDTH-1 || y > SSD1306_HEIGHT-1)
		return;
	page = y/SSD1306_PAGE_HEIGHT;
	ssd1306_buf[1 + page*SSD1306_WIDTH + x] ^=
		1u << y % SSD1306_PAGE_HEIGHT;
}

void
ssd1306_clear(void)
{
	memset(ssd1306_buf+1, 0x00, SSD1306_BUF_LEN-1);
}
