#include "suzuka.h"
#include "server.h"
#include "http.h"
#include "log.h"
#include "handler.h"
#include <signal.h>

int main(int argc, char *argv[])
{
  signal(SIGPIPE , SIG_IGN);

  sk_server server;
  server.port = 8080;
  server.addr = "127.0.0.1";
  server.handler = sk_handler_simple_send;
  server.document_root = "../public";

  sk_server_start(&server);
  return 0;
}
