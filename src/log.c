#include "log.h"

void sk_log_error(char* str)
{
  fprintf(stderr, "%s\n", str);
}

void sk_log_perror(char* str)
{
  perror(str);
}

void sk_log_debug(char* str)
{
  fprintf(stderr, "%s\n", str);
}
