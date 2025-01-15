/* Aluno: Daniel de Lima Franceschetti */

#include "pico/stdlib.h"

/*
Alfabeto morse
A  .-        B  -...
C  -.-.      D  -..
E  .         F  ..-.
G  --.       H  ....
I  ..        J  .---
K  -.-       L  .-..
M  --        N  -.
O  ---       P  .--.
Q  --.-      R  .-.
S  ...       T  -
U  ..-       V  ...-
W  .--       X  -..-
Y  -.--      Z  --..
0  -----     1  .----
2  ..---     3  ...--
4  ....-     5  .....
6  -....     7  --...
8  ---..     9  ----.
.  .-.-.-    ,  --..--
?  ..--..    =  -...-
*/

/* GPIO 11 */
#define PIN_LED 11

/* Duração dos elementos em milisegundos: */
#define DURATION_DOT          200
#define DURATION_DASH         800
#define DURATION_INTER_LETTER 125
#define DURATION_LETTER_GAP   250
#define DURATION_WORD_GAP     1600 /* esse não é necessário na atividade */
#define DURATION_CYCLE        3000

#define forever for(;;)

/* Explicação do código morse em 'code': 
  *   '.' é o sinal curto;
  *   '-' é o sinal longo;
  *   1 espaço é a separação entre letras;
  *   2 espaços são a separação entre palavras.
  */
void morse_loop(const char *code) {
  forever {
    for (size_t i = 0; code[i]; ++i) {
      if (code[i] == '.') {
        gpio_put(PIN_LED, 1);
        sleep_ms(DURATION_DOT);
        gpio_put(PIN_LED, 0);
      } else if (code[i] == '-') {
        gpio_put(PIN_LED, 1);
        sleep_ms(DURATION_DASH);
        gpio_put(PIN_LED, 0);
      } else if (code[i] == ' ') {
        gpio_put(PIN_LED, 0);
        if (code[i+1] == ' ') {
          sleep_ms(DURATION_WORD_GAP - DURATION_INTER_LETTER);
          ++i;
        } else {
          sleep_ms(DURATION_LETTER_GAP - DURATION_INTER_LETTER);
        }
      }
      sleep_ms(DURATION_INTER_LETTER);
    }
    sleep_ms(DURATION_CYCLE);
  }
}

int main(void) {
  gpio_init(PIN_LED);
  gpio_set_dir(PIN_LED, GPIO_OUT);
  morse_loop("... --- ..."); /* SOS */
}
