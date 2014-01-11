#include "file.h"

const char* sk_file_content_type(const char* path)
{
  const char* ext = sk_file_extname(path);
  return sk_file_content_type_ext(ext);
}

const char* sk_file_content_type_ext(const char* ext)
{
  if(strcmp(ext, "jpg") == 0) {
    return "image/jpeg";
  } else if(strcmp(ext, "jpeg") == 0) {
    return "image/jpeg";
  } else if(strcmp(ext, "png") == 0) {
    return "image/png";
  }
  return "text/html";
}

const char* sk_file_extname(const char* path)
{
  int n = strlen(path);
  for (int i = n-1; i >= 0; i--){
    if(path[i] == '.') {
      return &path[i+1];
    }
  }
  return "";
}

bool sk_file_validate_path(const char* path)
{
  /* ex. GET noslash.html */
  if(path[0] != '/') {
    return false;
  }
  int n = strlen(path);
  for (int i = 0; i < n; i++){
    if(path[i] == '/') {
      /* ex. GET /double//shash */
      if(path[i+1] && path[i+1] == '/') {
        return false;
      }
      /* ex. GET /directory/../traversal */
      if(path[i+1] && path[i+2] && path[i+1] == '.' && path[i+2] == '.') {
        return false;
      }
    }
  }
  return true;
}
