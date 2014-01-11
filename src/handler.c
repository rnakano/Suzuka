#include <sys/socket.h>
#include <sys/stat.h> 
#include "handler.h"
#include "http.h"
#include "log.h"
#include "file.h"
#include "sock.h"

void sk_handler_send_file(FILE* sock_fp, char* rpath)
{
  char path[256];
  sprintf(path, "../public%s", rpath);
  sk_sock_send_file(sock_fp, path);
}

void sk_handler_simple_send(int sockfd)
{
  char buff[256];
  FILE* sock_fp = fdopen(sockfd, "r+");
  sk_http_header* header = NULL;
  
  if(fgets(buff, 256, sock_fp) == NULL){
    sk_log_debug("connection closed");
    goto end_handler;
  }

  header = sk_http_parse_header(buff);
  if(header == NULL) {
    sk_log_debug("parse error.");
    goto end_handler;
  }
  sk_log_debug(header->path);
  if(! sk_file_validate_path(header->path)) {
    sk_sock_send_404(sock_fp);
    goto end_handler;
  }
  sk_handler_send_file(sock_fp, header->path);

end_handler:
  if(header) sk_http_header_free(header);
  fclose(sock_fp);
  close(sockfd);
}
