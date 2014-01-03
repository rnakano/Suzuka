#include "server.h"
#include "log.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void sk_server_start(sk_server* server)
{
  int server_sock;

  if((server_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0){
    sk_log_perror("cannot open server socket");
    exit(1);
  }

  socklen_t sin_siz = sizeof(struct sockaddr_in);
  struct sockaddr_in serv, clt;
  serv.sin_family = PF_INET;
  serv.sin_port = htons(server->port);
  serv.sin_addr.s_addr = INADDR_ANY;

  char yes = 1;
  setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes));

  if(bind(server_sock, (struct sockaddr*)&serv, sizeof(serv)) < 0){
    sk_log_perror("cannot bind");
    exit(1);
  }

  if(listen(server_sock, SOMAXCONN) < 0){
    sk_log_perror("cannot listen");
    exit(1);
  }

  int client_sock;
  while(true) {
    if((client_sock = accept(server_sock, (struct sockaddr*)&clt, &sin_siz)) < 0){
      sk_log_perror("cannot accept");
    }
    server->handler(client_sock);
  }
}
