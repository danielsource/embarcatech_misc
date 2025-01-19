#pragma once

/* calc.c */
bool calc(const char *expression, long *value);

/* calc_stack.c */
#define CALC_STACK_SIZE 256
extern int calc_top;
bool calc_push(long value);
bool calc_pop(long *value);

/* util.c */
void fatal_error(const char *message);
char *get_user_line(int *length);
