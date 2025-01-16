#include <stdlib.h>
#include <stdbool.h>

#include "cnpf.h"

/* Macro para verificar se caractere é um dígito. */
#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')

static bool calc_add2_(void)
{
  long x, y;

  if (!calc_pop(&y) || !calc_pop(&x)) {
    return false;
  }

  return calc_push(x + y);
}

static bool calc_sub2_(void)
{
  long x, y;

  if (!calc_pop(&y) || !calc_pop(&x)) {
    return false;
  }

  return calc_push(x - y);
}

static bool calc_mul2_(void)
{
  long x, y;

  if (!calc_pop(&y) || !calc_pop(&x)) {
    return false;
  }

  return calc_push(x * y);
}

static bool calc_div2_(void)
{
  long x, y;

  if (!calc_pop(&y) || !calc_pop(&x)) {
    return false;
  }

  if (y == 0) {
    return false;
  }

  return calc_push(x / y);
}

static bool calc_exp2_(void)
{
  long x, y;
  long power = 1;

  if (!calc_pop(&y) || !calc_pop(&x)) {
    return false;
  }

  if (y < 0) {
    return false;
  }

  while (y > 0) {
    power *= x;
    y--;
  }

  return calc_push(power);
}

static bool calc_fac1_(void)
{
    long x, i;
    long factorial = 1;

    if (!calc_pop(&x)) {
        return false;
    }

    if (x < 0) {
        return false;
    }

    for (i = 1; i <= x; ++i) {
        factorial *= i;
    }

    return calc_push(factorial);
}

/* A função 'calc' recebe uma cadeia de caracteres de uma expressão e um
 * ponteiro para um inteiro usado para armazenar o valor do cálculo.
 * Retorna 'true' se realizar o cálculo corretamente e 'false' ao encontrar
 * algum erro de sintaxe na expressão ou divisão por zero.
 *
 * A expressão recebida por 'calc' segue vagamente essa gramática em BNF:
 *   expression ::= integer    " "+ integer    " "+ operator |
 *                  integer    " "+ expression " "+ operator |
 *                  expression " "+ integer    " "+ operator |
 *                  expression " "+ expression " "+ operator
 *   operator   ::= "+" | "-" | "*" | "/"
 *   integer    ::= "-"? ("0" | [1-9] [0-9]*)
 */
bool calc(const char *expression, long *value)
{
  const char *s;
  char *n_end;
  long n;

  /* Comece com a pilha na posição inicial (vazia). */
  calc_top = -1;

  s = expression;
  *value = 0;

  /* Processe a expressão até chegar no seu fim: */
  while (s[0] != '\0') {

    /* Caso encontre um espaço: */
    if (s[0] == ' ') {
      /* Ignore o espaço e vá para o próximo caractere. */
      ++s;
      continue;
    }

    /* Caso encontre um inteiro: */
    if (IS_DIGIT(s[0]) || (s[0] == '-' && IS_DIGIT(s[1]))) {
      /* Tente converter os caracteres para um valor inteiro: */
      n = strtol(s, &n_end, 10);

      /* Finalize com erro caso encontre um caractere inesperado no final do número: */
      if (n_end[0] != ' ') {
        return false;
      }

      /* Finalize com erro caso não tenha mais espaço na pilha (veja 'CALC_STACK_SIZE'): */
      if (!calc_push(n)) {
        return false;
      }

      /* Pule todos os caracteres já processados por 'strtol': */
      s = n_end;
      continue;
    }

    /* Tente computar uma operação: */
    switch (s[0]) {
    /* As operações são dessa forma:
     *             calc_[nome][aridade]_ */
    case '+': if (!calc_add2_()) return false; break;
    case '-': if (!calc_sub2_()) return false; break;
    case '*': if (!calc_mul2_()) return false; break;
    case '/': if (!calc_div2_()) return false; break;
    case '^': if (!calc_exp2_()) return false; break;
    case '!': if (!calc_fac1_()) return false; break;
    default: /* Finalize com erro em um caractere inesperado: */ return false;
    }

    /* Finalize com erro caso encontre um caractere inesperado depois do operador: */
    if (s[1] != '\0' && s[1] != ' ') {
        return false;
    }

    ++s;
  }

  /* Finalize com erro se a pilha não estiver com apenas um elemento: */
  if (calc_top != 0) {
    return false;
  }

  /* Tente retirar o último elemento da pilha, passando o valor para 'value': */
  return calc_pop(value);
}
