#include "http.h"
#include "memory.h"

sk_http_header* sk_http_parse_header(char* str)
{
  char *type, *path, *version;
  char* saveptr;
  type = strtok_r(str, " \r\n", &saveptr);
  if(type == NULL) return NULL;
  if(strcmp(type, "GET") != 0) return NULL;

  path = strtok_r(NULL, " \r\n", &saveptr);
  if(path == NULL) return NULL;

  version = strtok_r(NULL, " \r\n", &saveptr);
  if(version == NULL) return NULL;
  if(strcmp(version, "HTTP/1.1") != 0) return NULL;

  sk_http_header *header = sk_malloc(sizeof(sk_http_header));
  strncpy(header->path, path, PATHLEN);
  return header;
}

void sk_http_header_free(sk_http_header* header)
{
  sk_free(header);
}

void sk_http_write_response_header(sk_http_response* response, char* buff, size_t buflen)
{
  snprintf(buff,
           buflen,
          "HTTP/1.1 %d OK" NL
          "Content-Type: %s" NL
          "Content-Length: %lld" NL
          "Connection: close" NL
          "Server: suzuka" NL NL,
          response->status_code,
          response->content_type,
          response->content_length);
}
