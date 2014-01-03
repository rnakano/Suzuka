#include "memory.h"
#include "log.h"

void* sk_malloc(size_t size) {
  void* ptr = malloc(size);
  if(ptr == NULL) {
    sk_log_error("Cannot allocate memory.");
    exit(1);
  }
  return ptr;
}
