#ifndef _SERVER_H_
#define _SERVER_H_

#include "suzuka.h"

typedef struct {
  unsigned short port;
  char* addr;
  void (*handler)(int);
} sk_server;

void sk_server_start(sk_server* server);

#endif /* _SERVER_H_ */
