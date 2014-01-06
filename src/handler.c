#include <sys/socket.h>
#include <sys/stat.h> 
#include "handler.h"
#include "http.h"
#include "log.h"
#include "file.h"

int sk_send(FILE* sock_fp, char* buf, int len)
{
/* #ifdef MSG_NOSIGNAL */
/*   int l = send(sockfd, buf, len, MSG_NOSIGNAL); */
/* #else */
/*   int l = send(sockfd, buf, len, SO_NOSIGPIPE); */
/* #endif */
  return fwrite(buf, 1, len, sock_fp);
}

#define RESPONSE_404 "HTTP/1.1 404 Not Found" NL NL

void sk_handler_send_404(FILE* sock_fp)
{
  sk_send(sock_fp, RESPONSE_404, strlen(RESPONSE_404));
}

void sk_handler_send_file(FILE* sock_fp, char* rpath)
{
  char path[256];
  sprintf(path, "../public%s", rpath);

  struct stat file_stat;
  stat(path, &file_stat);

  FILE* fp;
  if((fp = fopen(path, "rb")) == NULL) {
    sk_log_debug("file open error.");
    sk_handler_send_404(sock_fp);
    return;
  }
  
  char buff[1024];
  sk_http_response response = {
    .status_code = 200,
    .content_type = sk_file_content_type(rpath),
    .content_length = file_stat.st_size
  };
  sk_http_write_response_header(&response, buff, 1024);
  sk_send(sock_fp, buff, strlen(buff));

  size_t readbytes;
  while( (readbytes = fread(buff, 1, 1024, fp)) > 0) {
    sk_send(sock_fp, buff, readbytes);
  }
  fclose(fp);
}

void sk_handler_simple_send(int sockfd)
{
  char buff[256];
  FILE* sock_fp = fdopen(sockfd, "r+");
  
  if(fgets(buff, 256, sock_fp) == NULL){
    sk_log_debug("connection closed");
    return;
  }

  sk_http_header* header = sk_http_parse_header(buff);
  if(header == NULL) {
    sk_log_debug("parse error.");
    fclose(sock_fp);
    close(sockfd);
    return;
  }
  sk_log_debug(header->path);
  sk_handler_send_file(sock_fp, header->path);

  sk_http_header_free(header);
  fclose(sock_fp);
  close(sockfd);
}
