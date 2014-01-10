#include <sys/socket.h>
#include <sys/stat.h> 
#include "sock.h"
#include "http.h"
#include "file.h"

#define RESPONSE_404 "HTTP/1.1 404 Not Found" NL NL

int sk_sock_send(FILE* sock_fp, char* buf, int len)
{
  return fwrite(buf, 1, len, sock_fp);
}

void sk_sock_send_file(FILE* sock_fp, char* path)
{
  struct stat file_stat;
  stat(path, &file_stat);

  FILE* file_fp;
  if((file_fp = fopen(path, "rb")) == NULL) {
    sk_sock_send_404(sock_fp);
    return;
  }

  char buff[1024];
  sk_http_response response = {
    .status_code = 200,
    .content_type = sk_file_content_type(path),
    .content_length = file_stat.st_size
  };
  sk_http_write_response_header(&response, buff, 1024);
  sk_sock_send(sock_fp, buff, strlen(buff));

  size_t readbytes;
  while( (readbytes = fread(buff, 1, 1024, file_fp)) > 0) {
    sk_sock_send(sock_fp, buff, readbytes);
  }
}

void sk_sock_send_404(FILE* sock_fp)
{
  sk_sock_send(sock_fp, RESPONSE_404, strlen(RESPONSE_404));
}
