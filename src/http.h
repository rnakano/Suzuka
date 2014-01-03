#ifndef _HTTP_H_
#define _HTTP_H_

#include "suzuka.h"

#define PATHLEN 256

typedef struct {
  char path[PATHLEN];
} sk_http_header;

sk_http_header* sk_http_parse_header(char*);
void sk_http_header_free(sk_http_header*);

#endif /* _HTTP_H_ */
