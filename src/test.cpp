#include <gtest/gtest.h>
#include <string>

using namespace std;

extern "C" {
  #include <string.h>
  #include <alloca.h>
  #include "http.h"
  #include "handler.h"
}

#define cstring(varname, x) \
  string varname##_ (x); \
  char* varname = (char*) alloca(strlen( (varname##_) .c_str())); \
  strcpy(varname, (varname##_).c_str()); \

TEST(HttpTest, parseError)
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

TEST(HttpTest, parse)
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

TEST(HandlerTest, extname)
{
  cstring(input1, "hoge.jpg");
  char buff[8];
  sk_handler_extname(input1, buff);
  EXPECT_STREQ("jpg", buff);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
