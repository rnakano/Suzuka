#include "log.h"

void sk_log_error(char* str)
{
  fprintf(stderr, "%s\n", str);
}
