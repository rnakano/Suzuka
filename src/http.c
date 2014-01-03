#include "http.h"
#include "memory.h"

sk_http_header* sk_http_parse_header(char* str)
{
  char *type, *path, *version;
  char* saveptr;
  type = strtok_r(str, " ", &saveptr);
  if(type == NULL) return NULL;
  if(strcmp(type, "GET") != 0) return NULL;

  path = strtok_r(NULL, " ", &saveptr);
  if(path == NULL) return NULL;

  version = strtok_r(NULL, " ", &saveptr);
  if(version == NULL) return NULL;
  if(strcmp(version, "HTTP/1.1") != 0) return NULL;

  sk_http_header *header = sk_malloc(sizeof(sk_http_header));
  strncpy(header->path, path, PATHLEN);
  return header;
}
