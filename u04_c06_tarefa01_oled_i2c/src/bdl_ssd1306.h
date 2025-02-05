/* NOTE: this code targets BitDogLab v6.3 using its OLED I2C 128x64 display;
         based on this example: https://github.com/raspberrypi/pico-examples/tree/master/i2c/ssd1306_i2c
*/

#define SSD1306_WIDTH  128
#define SSD1306_HEIGHT 64
#define SSD1306_FONT_WIDTH 6  /* height is always 8 */
#define SSD1306_I2C_PORT i2c1
#define SSD1306_I2C_SDA  14
#define SSD1306_I2C_SCL  15

#define SSD1306_I2C_ADDR            _u(0x3C)
#define SSD1306_I2C_CLK             400
#define SSD1306_SET_MEM_MODE        _u(0x20)
#define SSD1306_SET_COL_ADDR        _u(0x21)
#define SSD1306_SET_PAGE_ADDR       _u(0x22)
#define SSD1306_SET_HORIZ_SCROLL    _u(0x26)
#define SSD1306_SET_SCROLL          _u(0x2E)
#define SSD1306_SET_DISP_START_LINE _u(0x40)
#define SSD1306_SET_CONTRAST        _u(0x81)
#define SSD1306_SET_CHARGE_PUMP     _u(0x8D)
#define SSD1306_SET_SEG_REMAP       _u(0xA0)
#define SSD1306_SET_ENTIRE_ON       _u(0xA4)
#define SSD1306_SET_ALL_ON          _u(0xA5)
#define SSD1306_SET_NORM_DISP       _u(0xA6)
#define SSD1306_SET_INV_DISP        _u(0xA7)
#define SSD1306_SET_MUX_RATIO       _u(0xA8)
#define SSD1306_SET_DISP            _u(0xAE)
#define SSD1306_SET_COM_OUT_DIR     _u(0xC0)
#define SSD1306_SET_COM_OUT_DIR_FLIP _u(0xC0)
#define SSD1306_SET_DISP_OFFSET     _u(0xD3)
#define SSD1306_SET_DISP_CLK_DIV    _u(0xD5)
#define SSD1306_SET_PRECHARGE       _u(0xD9)
#define SSD1306_SET_COM_PIN_CFG     _u(0xDA)
#define SSD1306_SET_VCOM_DESEL      _u(0xDB)
#define SSD1306_PAGE_HEIGHT         _u(8)
#define SSD1306_NUM_PAGES           (SSD1306_HEIGHT / SSD1306_PAGE_HEIGHT)
#define SSD1306_BUF_LEN             (SSD1306_NUM_PAGES * SSD1306_WIDTH)
#define SSD1306_WRITE_MODE         _u(0xFE)
#define SSD1306_READ_MODE          _u(0xFF)

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

enum {
	ssd1306_TextArea_Line = SSD1306_WIDTH / SSD1306_FONT_WIDTH,
	ssd1306_TextArea_Size = (SSD1306_HEIGHT/8 - 2)*ssd1306_TextArea_Line
};

typedef struct {
    uint8_t start_col;
    uint8_t end_col;
    uint8_t start_page;
    uint8_t end_page;

    int buflen;
} ssd1306_RenderArea;

typedef struct {
	uint8_t data[ssd1306_TextArea_Size];
	uint8_t cursor;
} ssd1306_TextArea;

static void ssd1306_init(void);
static void ssd1306_update(void);
static void ssd1306_put_char(uint8_t ch);
static void ssd1306_del_char(void);
static void ssd1306_put_info(const char *msg);

static const uint8_t ssd1306_font[ssd1306_CharCount*SSD1306_FONT_WIDTH] = {
#include "font.inc"
};

static ssd1306_RenderArea ssd1306_frame_area = {
	.start_col = 0,
	.end_col = SSD1306_WIDTH - 1,
	.start_page = 0,
	.end_page = SSD1306_NUM_PAGES - 1
};

static ssd1306_TextArea ssd1306_text;

static uint8_t ssd1306_buf[SSD1306_BUF_LEN];

static void
ssd1306_send_cmd(uint8_t cmd)
{
	uint8_t buf[2] = {0x80, cmd};
	i2c_write_blocking(SSD1306_I2C_PORT, SSD1306_I2C_ADDR, buf, 2, false);
}

static void
ssd1306_send_cmd_list(uint8_t *buf, int num)
{
	for (int i = 0; i < num; ++i)
		ssd1306_send_cmd(buf[i]);
}

static void
ssd1306_send_buf(uint8_t *buf, int buflen)
{
    uint8_t *temp_buf = malloc(buflen + 1);
    temp_buf[0] = 0x40;
    memcpy(temp_buf+1, buf, buflen);
    i2c_write_blocking(SSD1306_I2C_PORT, SSD1306_I2C_ADDR, temp_buf, buflen + 1, false);
    free(temp_buf);
}

static void
ssd1306_render(uint8_t *buf, ssd1306_RenderArea *area) {
	uint8_t cmds[] = {
		SSD1306_SET_COL_ADDR,
		area->start_col,
		area->end_col,
		SSD1306_SET_PAGE_ADDR,
		area->start_page,
		area->end_page
	};
	ssd1306_send_cmd_list(cmds, count_of(cmds));
	ssd1306_send_buf(buf, area->buflen);
}

void
ssd1306_init(void)
{
	i2c_init(SSD1306_I2C_PORT, SSD1306_I2C_CLK*1000);
	gpio_set_function(SSD1306_I2C_SDA, GPIO_FUNC_I2C);
	gpio_set_function(SSD1306_I2C_SCL, GPIO_FUNC_I2C);
	gpio_pull_up(SSD1306_I2C_SDA);
	gpio_pull_up(SSD1306_I2C_SCL);

	uint8_t cmds[] = {
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
		0x12,                           /* 0x02 works for 128x32, 0x12 possibly works for 128x64; other options 0x22, 0x32 */
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

	ssd1306_send_cmd_list(cmds, count_of(cmds));

	ssd1306_frame_area.buflen = (ssd1306_frame_area.end_col - ssd1306_frame_area.start_col + 1) 
		* (ssd1306_frame_area.end_page - ssd1306_frame_area.start_page + 1);

	ssd1306_update();
}

void
ssd1306_update(void)
{
	ssd1306_render(ssd1306_buf, &ssd1306_frame_area);
}

void
ssd1306_put_char(uint8_t ch)
{
	ssd1306_text.data[ssd1306_text.cursor++] = ch;
}

void
ssd1306_del_char(void)
{
	--ssd1306_text.cursor;
}

void
ssd1306_put_info(const char *msg)
{
	uint8_t x = 0, y = SSD1306_HEIGHT/8 - 1;
	uint8_t ch;

	memset(ssd1306_buf + (y-1)*SSD1306_WIDTH, 0x10, SSD1306_WIDTH);
	memset(ssd1306_buf + y*SSD1306_WIDTH, 0x00, SSD1306_WIDTH);

	while ((ch = *msg++) != '\0') {
		if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
			ch -= 'A';
		} else if (ch >= '0' && ch <= '9') {
			ch = ch-'0' + ssd1306_Char_0;
		} else {
			ch = ssd1306_Char_Space;
		}

		for (int i = 0; i < SSD1306_FONT_WIDTH; ++i) {
			ssd1306_buf[y*SSD1306_WIDTH + x + i] = ssd1306_font[ch*SSD1306_FONT_WIDTH + i];
		}

		x += SSD1306_FONT_WIDTH;
	}
}
