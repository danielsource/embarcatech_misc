#include <stdio.h>
#include <stdlib.h>

#include "cnpf.h"

/* Imprima mensagem de erro e finalize a execução com código de falha (1). */
void fatal_error(const char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

/* Obtenha caracteres da linha de entrada. */
char *get_user_line(int *length)
{
  static char buffer[512];
  int i;

  fputs("> ", stdout);
  fflush(stdout);

  if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
    fatal_error("fgets() falhou.\n");
  }

  /* Remova a "nova linha" do final da cadeia. */
  for (i = 0; buffer[i] != '\0'; ++i) {
    if (buffer[i] == '\n') {
      buffer[i] = '\0';
      break;
    }
  }

  *length = i;
  return buffer;
}
