#include "image.h"

void sk_image_parse_term(char* term, sk_image_request* result)
{
  char* saveptr;
  char* name = strtok_r(term, "=", &saveptr);
  if(name == NULL) return;
  if(strcmp(name, "w") == 0) {
    if(strcmp(saveptr, "auto") == 0) {
      result->convert_flag |= CONVERT_WIDTH_AUTO;
    } else {
      uint32_t width;
      if(sscanf(saveptr, "%ud", &width) > 0) {
        result->convert_flag &= ~CONVERT_WIDTH_AUTO;
        result->width = width;
      }
    }
  } else if(strcmp(name, "h") == 0) {
    if(strcmp(saveptr, "auto") == 0) {
      result->convert_flag |= CONVERT_HEIGHT_AUTO;
    } else {
      uint32_t height;
      if(sscanf(saveptr, "%ud", &height) > 0) {
        result->convert_flag &= ~CONVERT_HEIGHT_AUTO;
        result->height = height;
      }
    }
  }
}

void sk_image_parse_request(char* request, sk_image_request* result) 
{
  char* saveptr;
  char* path = strtok_r(request, "?", &saveptr);
  if(saveptr == NULL) {
    result->path = request;
    result->convert_flag |= CONVERT_NONE;
    return;
  }
  result->path = path;

  char* term;
  while((term = strtok_r(NULL, "&", &saveptr)) != NULL) {
    sk_image_parse_term(term, result);
  }
}
