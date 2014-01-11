#ifndef _SERVER_H_
#define _SERVER_H_

#include "suzuka.h"

typedef struct sk_server_T sk_server;

struct sk_server_T {
  unsigned short port;
  char* addr;
  char* document_root;
  void (*handler)(struct sk_server_T*, int);
};

void sk_server_start(sk_server* server);

#endif /* _SERVER_H_ */
