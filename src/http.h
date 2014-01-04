#ifndef _HTTP_H_
#define _HTTP_H_

#include "suzuka.h"

#define PATHLEN 256

typedef struct {
  char path[PATHLEN];
} sk_http_header;

typedef struct {
  unsigned short status_code;
  const char* content_type;
  uint64_t content_length;
} sk_http_response;

sk_http_header* sk_http_parse_header(char*);
void sk_http_header_free(sk_http_header*);
void sk_http_write_response_header(sk_http_response*, char*, size_t);

#endif /* _HTTP_H_ */
