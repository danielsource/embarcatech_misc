#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "bdl_ssd1306.h"

#define LENGTH(arr) (sizeof(arr)/sizeof((arr)[0]))

enum {
	ssd1306_Char_A = 0,
	ssd1306_Char_B,
	ssd1306_Char_C,
	ssd1306_Char_D,
	ssd1306_Char_E,
	ssd1306_Char_F,
	ssd1306_Char_G,
	ssd1306_Char_H,
	ssd1306_Char_I,
	ssd1306_Char_J,
	ssd1306_Char_K,
	ssd1306_Char_L,
	ssd1306_Char_M,
	ssd1306_Char_N,
	ssd1306_Char_O,
	ssd1306_Char_P,
	ssd1306_Char_Q,
	ssd1306_Char_R,
	ssd1306_Char_S,
	ssd1306_Char_T,
	ssd1306_Char_U,
	ssd1306_Char_V,
	ssd1306_Char_W,
	ssd1306_Char_X,
	ssd1306_Char_Y,
	ssd1306_Char_Z,

	ssd1306_Char_a,
	ssd1306_Char_b,
	ssd1306_Char_c,
	ssd1306_Char_d,
	ssd1306_Char_e,
	ssd1306_Char_f,
	ssd1306_Char_g,
	ssd1306_Char_h,
	ssd1306_Char_i,
	ssd1306_Char_j,
	ssd1306_Char_k,
	ssd1306_Char_l,
	ssd1306_Char_m,
	ssd1306_Char_n,
	ssd1306_Char_o,
	ssd1306_Char_p,
	ssd1306_Char_q,
	ssd1306_Char_r,
	ssd1306_Char_s,
	ssd1306_Char_t,
	ssd1306_Char_u,
	ssd1306_Char_v,
	ssd1306_Char_w,
	ssd1306_Char_x,
	ssd1306_Char_y,
	ssd1306_Char_z,

	ssd1306_Char_0,
	ssd1306_Char_1,
	ssd1306_Char_2,
	ssd1306_Char_3,
	ssd1306_Char_4,
	ssd1306_Char_5,
	ssd1306_Char_6,
	ssd1306_Char_7,
	ssd1306_Char_8,
	ssd1306_Char_9,

	ssd1306_Char_Space,
	ssd1306_CharCount
};

typedef struct {
	uint8_t data[SSD1306_TEXTAREA_SIZE];
	uint8_t cursor;
} ssd1306_TextArea;

static const uint8_t ssd1306_font[ssd1306_CharCount * SSD1306_FONT_WIDTH] = {
#include "font.inc"
};

static ssd1306_TextArea ssd1306_text;

static uint8_t ssd1306_buf[SSD1306_BUF_LEN] = {0x40};

static void
ssd1306_send_cmd(uint8_t cmd)
{
	uint8_t buf[2] = {0x80};
	buf[1] = cmd;
	i2c_write_blocking(SSD1306_I2C_PORT, SSD1306_I2C_ADDR, buf, 2, false);
}

static void
ssd1306_send_cmd_list(uint8_t *buf, int num)
{
	int i;
	for (i = 0; i < num; ++i)
		ssd1306_send_cmd(buf[i]);
}

static void
ssd1306_send_buf(uint8_t *buf, int buflen)
{
	i2c_write_blocking(SSD1306_I2C_PORT, SSD1306_I2C_ADDR, buf, buflen, false);
}

static uint8_t
ssd1306_char_idx(char ch)
{
	if (ch >= 'A' && ch <= 'Z') {
		return ch-'A';
	} else if (ch >= 'a' && ch <= 'z') {
		return ch-'a' + ssd1306_Char_a;
	} else if (ch >= '0' && ch <= '9') {
		return ch-'0' + ssd1306_Char_0;
	} else {
		return ssd1306_Char_Space;
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
	ssd1306_send_cmd_list(cmds, LENGTH(cmds));
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
	ssd1306_send_cmd_list(cmds, LENGTH(cmds));
	ssd1306_send_buf(ssd1306_buf, SSD1306_BUF_LEN);
}

void
ssd1306_put_char(char ch_)
{
	uint8_t x, y;
	uint8_t ch;
	int i;

	if (ssd1306_text.cursor == SSD1306_TEXTAREA_SIZE)
		return;

	ssd1306_text.data[ssd1306_text.cursor] = ch = ssd1306_char_idx(ch_);

	x = ssd1306_text.cursor % SSD1306_TEXTAREA_LINE * SSD1306_FONT_WIDTH;
	y = ssd1306_text.cursor / SSD1306_TEXTAREA_LINE;

	for (i = 0; i < SSD1306_FONT_WIDTH; ++i)
		ssd1306_buf[1 + y*SSD1306_WIDTH + x + i] =
			ssd1306_font[ch*SSD1306_FONT_WIDTH + i];

	++ssd1306_text.cursor;
}

void
ssd1306_put_str(const char *str)
{
	while (ssd1306_text.cursor < SSD1306_TEXTAREA_SIZE && *str)
		ssd1306_put_char(*str++);
}

void
ssd1306_del_char(void)
{
	uint8_t x, y;

	if (ssd1306_text.cursor == 0)
		return;

	--ssd1306_text.cursor;

	x = ssd1306_text.cursor % SSD1306_TEXTAREA_LINE * SSD1306_FONT_WIDTH;
	y = ssd1306_text.cursor / SSD1306_TEXTAREA_LINE;

	memset(ssd1306_buf+1 + y*SSD1306_WIDTH + x, 0x00, SSD1306_FONT_WIDTH);
}

void
ssd1306_clr_chars(void)
{
	while (ssd1306_text.cursor)
		ssd1306_del_char();
}

void
ssd1306_put_info(const char *msg)
{
	uint8_t x = 0, y = SSD1306_NUM_PAGES-1;
	uint8_t ch;
	int i;

	memset(ssd1306_buf+1 + (y-1)*SSD1306_WIDTH, 0x10, SSD1306_WIDTH);
	memset(ssd1306_buf+1 + y*SSD1306_WIDTH, 0x00, SSD1306_WIDTH);

	while (*msg != '\0') {
		ch = ssd1306_char_idx(*msg++);
		for (i = 0; i < SSD1306_FONT_WIDTH; ++i)
			ssd1306_buf[1 + y*SSD1306_WIDTH + x + i] =
				ssd1306_font[ch*SSD1306_FONT_WIDTH + i];
		x += SSD1306_FONT_WIDTH;
	}
}
