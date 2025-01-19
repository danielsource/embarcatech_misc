#include <stdbool.h>

#include "cnpf.h"

/* "Apontador" para o topo da pilha. */
int calc_top = -1;

/* Pilha para armazenar operandos. */
static long calc_stack_[CALC_STACK_SIZE];

/* Empilhar valor na pilha da calculadora. */
bool calc_push(long value)
{
  if (calc_top < CALC_STACK_SIZE-1) {
    calc_stack_[++calc_top] = value;
    return true;
  } else {
    return false;
  }
}

/* Desempilhar valor na pilha da calculadora. */
bool calc_pop(long *value)
{
  if (calc_top >= 0)
  {
    *value = calc_stack_[calc_top--];
    return true;
  } else {
    return false;
  }
}
