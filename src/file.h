#ifndef _FILE_H_
#define _FILE_H_

#include "suzuka.h"

const char* sk_file_content_type(const char* path);
const char* sk_file_content_type_ext(const char* ext);
const char* sk_file_extname(const char* path);
bool sk_file_validate_path(const char* path);

#endif /* _FILE_H_ */
