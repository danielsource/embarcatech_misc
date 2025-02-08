/*
LED logical positions:
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

Pixel format:
  0xGGRRBB00 (32 bits)
  GG is the green byte (8 bits)
  RR is the red byte   (8 bits)
  BB is the blue byte  (8 bits)
  00 are the unused least significant 8 bits
*/

void ws2812_init(void);
void ws2812_update(void);
void ws2812_fill(uint32_t color);
void ws2812_put_px(uint8_t x, uint8_t y, uint32_t color);
void ws2812_put_digit(char digit, uint32_t bg, uint32_t fg);
