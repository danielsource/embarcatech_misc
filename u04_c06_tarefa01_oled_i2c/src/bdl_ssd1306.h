#define SSD1306_WIDTH       128
#define SSD1306_HEIGHT      64
#define SSD1306_FONT_WIDTH  6  /* height is always SSD1306_PAGE_HEIGHT */
#define SSD1306_I2C_PORT    i2c1
#define SSD1306_I2C_SDA     14
#define SSD1306_I2C_SCL     15
#define SSD1306_COM_PIN_CFG 0x12

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
#define SSD1306_BUF_LEN             (SSD1306_NUM_PAGES * SSD1306_WIDTH + 1 /*for cmd*/)
#define SSD1306_WRITE_MODE          _u(0xFE)
#define SSD1306_READ_MODE           _u(0xFF)

#define SSD1306_TEXTAREA_LINE (SSD1306_WIDTH / SSD1306_FONT_WIDTH)
#define SSD1306_TEXTAREA_SIZE ((SSD1306_NUM_PAGES - 2) * SSD1306_TEXTAREA_LINE)

void ssd1306_init(void);
void ssd1306_update(void);
void ssd1306_put_char(char ch);
void ssd1306_put_str(const char *str);
void ssd1306_del_char(void);
void ssd1306_clr_chars(void);
void ssd1306_put_info(const char *msg);
