const char *const banner =
  "cnpf: calculadora em notação pós-fixada\n"
  "==========================================\n"
  "'cnpf' é uma simples calculadora de linha\n"
  "de comando que implementa as 4 operações\n"
  "aritméticas (+ - * /) e mais outras em\n"
  "inteiros na notação pós-fixada (também\n"
  "(conhecida como 'Reverse Polish Notation).\n"
  "==========================================\n"
  "DIGITE 'q' PARA SAIR | DIGITE 'e' PARA EXEMPLOS"
;

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>

#include "cnpf.h"

#include "util.c"
#include "calc_stack.c"
#include "calc.c"

int main(int argc, char **argv)
{
  char *expression;
  int expression_length;
  long value;
  int i;

  setlocale(LC_ALL, "");

  if (argc == 2 && strcmp(argv[1], "-q") == 0) {
    /* Desativar 'banner' inicial ao passar a opção "-q". */
  } else if (argc < 2) {
    puts(banner);
  } else {
    fatal_error("Utilização: cnpf [-q]");
  }

  while (true) {
    /* Obtenha a expressão aritmética do usuário. */
    expression = get_user_line(&expression_length);

    /* Remova espaços desnecessários no *início* da expressão. */
    while (expression[0] == ' ') {
      ++expression;
      --expression_length;
    }

    /* Remova espaços desnecessários no *final* da expressão. */
    for (i = expression_length-1; expression[i] == ' '; --i) {
      expression[i] = '\0';
      --expression_length;
    }

    /* Se usuário não digitar nada, prossiga para a próxima leitura. */
    if (expression_length == 0) {
      continue;
    }
    /* Se usuário digitar 'q', finalize o programa (quit). */
    else if (strcmp(expression, "q") == 0) {
      break;
    }
    /* Se usuário digitar 'e', exiba exemplos da calculadora. */
    else if (strcmp(expression, "e") == 0) {
      puts("\n"
           "Expressão convencional: 2 + 2\n"
           "Expressão 'cnpf':       2 2 +\n"
           "\n"
           "Expressão convencional: (4 + 6) / 2\n"
           "Expressão 'cnpf':       4 6 + 2 /\n"
           "\n"
           "Expressão convencional: ((4 * 2) - (5 * 2)) / (2 * -1)\n"
           "Expressão 'cnpf':       4 2 * 5 2 * - 2 -1 * /\n"
           "\n"
           "Expressão convencional: 2 ^ 3\n"
           "Expressão 'cnpf':       2 3 ^\n"
           "\n"
           "Expressão 'cnpf':       5 ! (fatorial de 5)\n"
           "\n"
           "Note que na notação pós-fixada não é usado parênteses.\n"
           "Obs: neste programa, espaços entre operandos ou operadores são obrigatórios!\n");
      continue;
    }

    /* Execute a calculadora 'calc': */
    if (calc(expression, &value)) {
      printf("%ld\n", value);
    } else {
      puts("<Erro de sintaxe>");
    }
  }

  return 0;
}
