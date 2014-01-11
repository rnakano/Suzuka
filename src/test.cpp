#include <gtest/gtest.h>
#include <string>

using namespace std;

extern "C" {
  #include <string.h>
  #include <alloca.h>
  #include "http.h"
  #include "file.h"
}

#define cstring(varname, x) \
  string varname##_ (x); \
  char* varname = (char*) alloca(strlen( (varname##_) .c_str())); \
  strcpy(varname, (varname##_).c_str()); \

TEST(http, parse_error)
{
  cstring(input1, "hogehoge");
  EXPECT_TRUE(sk_http_parse_header(input1) == NULL);

  cstring(input2, "BAD / HTTP/1.1");
  EXPECT_TRUE(sk_http_parse_header(input2) == NULL);

  cstring(input3, "GET HTTP/1.1");
  EXPECT_TRUE(sk_http_parse_header(input3) == NULL);

  cstring(input4, "GET  HTTP/1.1");
  EXPECT_TRUE(sk_http_parse_header(input4) == NULL);

  cstring(input5, "GET /some/path HTTP/2.0");
  EXPECT_TRUE(sk_http_parse_header(input5) == NULL);
}

TEST(http, parse)
{
  cstring(input1, "GET / HTTP/1.1");
  sk_http_header* header = sk_http_parse_header(input1);
  EXPECT_TRUE(header != NULL);
  EXPECT_STREQ("/", header->path);
  sk_http_header_free(header);

  #define NL "\r\n"
  
  cstring(input2, "GET / HTTP/1.1" NL
          "User-Agent: curl/7.24.0 (x86_64-apple-darwin12.0) libcurl/7.24.0 OpenSSL/0.9.8y zlib/1.2.5" NL
          "Host: localhost:8080" NL
          "Accept: */*" NL);
  header = sk_http_parse_header(input2);
  EXPECT_TRUE(header != NULL);
  EXPECT_STREQ("/", header->path);
  sk_http_header_free(header);
}

TEST(file, extname)
{
  cstring(jpg, "hoge.jpg");
  EXPECT_STREQ("jpg", sk_file_extname(jpg));

  cstring(no_ext, "no_ext");
  EXPECT_STREQ("", sk_file_extname(no_ext));
}

TEST(file, content_type_ext)
{
  cstring(jpg, "jpg");
  EXPECT_STREQ("image/jpeg", sk_file_content_type_ext(jpg));

  cstring(jpeg, "jpeg");
  EXPECT_STREQ("image/jpeg", sk_file_content_type_ext(jpeg));
}

TEST(file, content_type)
{
  cstring(jpg, "/img/hoge.jpg");
  EXPECT_STREQ("image/jpeg", sk_file_content_type(jpg));
}

TEST(file, validate_path)
{
  cstring(ok, "/hoge/img.jpg");
  EXPECT_TRUE(sk_file_validate_path(ok));

  cstring(index, "/");
  EXPECT_TRUE(sk_file_validate_path(index));

  cstring(root, "//dev/random");
  EXPECT_FALSE(sk_file_validate_path(root));

  cstring(parent, "/hoge/../some/dir");
  EXPECT_FALSE(sk_file_validate_path(parent));

  cstring(noslash, "hoge");
  EXPECT_FALSE(sk_file_validate_path(noslash));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
