#include <sys/socket.h>
#include <sys/stat.h> 
#include "handler.h"
#include "http.h"
#include "log.h"

int sk_send(int sockfd, char* buf, int len)
{
#ifdef MSG_NOSIGNAL
  int l = send(sockfd, buf, len, MSG_NOSIGNAL);
#else
  int l = send(sockfd, buf, len, SO_NOSIGPIPE);
#endif
  return l;
}

void sk_handler_send_404(int sockfd)
{
  sk_send(sockfd, "", 0);
}

char* sk_handler_content_type(char* ext)
{
  if(strcmp(ext, "jpg") == 0) {
    return "image/jpeg";
  }
  return "text/html";
}

void sk_handler_extname(char* path, char* ext)
{
  int n = strlen(path);
  int i;
  for (i = n-1; i >= 0; i--){
    if(path[i] == '.') break;
  }
  strcpy(ext, &path[i+1]);
}

void sk_handler_send_file(int sockfd, char* rpath)
{
  char path[256];
  sprintf(path, "../public%s", rpath);
  char ext[8];
  sk_handler_extname(rpath, ext);

  struct stat file_stat;
  stat(path, &file_stat);

  FILE* fp;
  if((fp = fopen(path, "rb")) == NULL) {
    sk_log_debug("file open error.");
    sk_handler_send_404(sockfd);
    return;
  }

  #define NL "\r\n"
  
  char* content_type = sk_handler_content_type(ext);
  char buff[1024];
  sprintf(buff,
          "HTTP/1.1 200 OK" NL
          "Content-Type: %s" NL
          "Content-Length: %lld" NL
          "Connection: close" NL
          "Server: suzuka" NL NL,
          content_type,
          file_stat.st_size);
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
