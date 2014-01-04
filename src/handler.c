#include <sys/socket.h>
#include <sys/stat.h> 
#include "handler.h"
#include "http.h"
#include "log.h"
#include "file.h"

int sk_send(int sockfd, char* buf, int len)
{
#ifdef MSG_NOSIGNAL
  int l = send(sockfd, buf, len, MSG_NOSIGNAL);
#else
  int l = send(sockfd, buf, len, SO_NOSIGPIPE);
#endif
  return l;
}

#define RESPONSE_404 "HTTP/1.1 404 Not Found" NL NL

void sk_handler_send_404(int sockfd)
{
  sk_send(sockfd, RESPONSE_404, strlen(RESPONSE_404));
}

void sk_handler_send_file(int sockfd, char* rpath)
{
  char path[256];
  sprintf(path, "../public%s", rpath);

  struct stat file_stat;
  stat(path, &file_stat);

  FILE* fp;
  if((fp = fopen(path, "rb")) == NULL) {
    sk_log_debug("file open error.");
    sk_handler_send_404(sockfd);
    return;
  }
  
  char buff[1024];
  sk_http_response response = {
    .status_code = 200,
    .content_type = sk_file_content_type(rpath),
    .content_length = file_stat.st_size
  };
  sk_http_write_response_header(&response, buff, 1024);
  sk_send(sockfd, buff, strlen(buff));

  size_t readbytes;
  while( (readbytes = fread(buff, 1, 1024, fp)) > 0) {
    sk_send(sockfd, buff, readbytes);
  }
  fclose(fp);
}

void sk_handler_simple_send(int sockfd)
{
  char buff[256];
  memset(buff, '\0', 256);

  if(recv(sockfd, buff, 256, 0) == 0){
    sk_log_debug("connection closed");
    return;
  }

  sk_http_header* header = sk_http_parse_header(buff);
  if(header == NULL) {
    sk_log_debug("parse error.");
    close(sockfd);
    return;
  }
  sk_log_debug(header->path);
  sk_handler_send_file(sockfd, header->path);

  sk_http_header_free(header);  
  close(sockfd);
}
