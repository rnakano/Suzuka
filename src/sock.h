#ifndef _SOCK_H_
#define _SOCK_H_

#include "suzuka.h"

int sk_sock_send(FILE* sock_fp, char* buf, int len);
void sk_sock_send_file(FILE* sock_fp, char* path);
void sk_sock_send_404(FILE* sock_fp);

#endif /* _SOCK_H_ */
