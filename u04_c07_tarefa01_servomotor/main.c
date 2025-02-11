/* Aluno: Daniel de Lima Franceschetti */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define PIN_SRV 12 /* servomotor */

/* gpio  | 0  | 1  | 2  | 3  | 4  | ... | 12 | 13 ...
pwm chan | 0A | 0B | 1A | 1B | 2A |     | 6A | 6B */
#define PWM_SLICE 6
#define PWM_CLKDIV 38.0f
#define PWM_WRAP 65535

int
main(void)
{
  stdio_init_all();

  gpio_set_function(PIN_SRV, GPIO_FUNC_PWM);
  pwm_set_clkdiv(PWM_SLICE, PWM_CLKDIV);
  pwm_set_wrap(PWM_SLICE, PWM_WRAP);
  pwm_set_enabled(PWM_SLICE, 1);

  sleep_ms(1000);
  printf("(1) frequência do pwm é ~50Hz (período de 20ms)\n");
  sleep_ms(1000);

  printf("(2) duty cycle de 12%%\n");
  pwm_set_gpio_level(PIN_SRV, PWM_WRAP*0.12f);
  sleep_ms(5000);

  printf("(3) duty cycle de 7,35%%\n");
  pwm_set_gpio_level(PIN_SRV, PWM_WRAP*0.0735f);
  sleep_ms(5000);

  printf("(4) duty cycle de 2,5%%\n");
  pwm_set_gpio_level(PIN_SRV, PWM_WRAP*0.025f);
  sleep_ms(5000);
  
  printf("(5) movimento periódico suave\n");
  int level = PWM_WRAP*0.025f;
  int incr = 0;
  for (;;) {
    if (incr) {
      level += PWM_WRAP*0.00025;
      if (level >= PWM_WRAP*0.12f)
        incr = !incr;
    } else {
      level -= PWM_WRAP*0.00025;
      if (level <= PWM_WRAP*0.025f)
        incr = !incr;
    }

    pwm_set_gpio_level(PIN_SRV, level);
    sleep_ms(10);
  }
}
