#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "suzuka.h"

#define CONVERT_NONE        0b0001
#define CONVERT_WIDTH_AUTO  0b0010
#define CONVERT_HEIGHT_AUTO 0b0100

typedef struct {
  uint32_t width, height;
  uint32_t convert_flag;
  char* path;
} sk_image_request;

void sk_image_parse_request(char* request, sk_image_request* result);
void sk_image_parse_term(char* term, sk_image_request* result);

#endif /* _IMAGE_H_ */
