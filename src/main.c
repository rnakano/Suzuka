#include "suzuka.h"
#include "server.h"
#include "http.h"
#include "log.h"
#include "handler.h"

int main(int argc, char *argv[])
{
  sk_server server;
  server.port = 8080;
  server.addr = "127.0.0.1";
  server.handler = sk_handler_simple_send;

  sk_server_start(&server);
  return 0;
}
