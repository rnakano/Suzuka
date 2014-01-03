#include <gtest/gtest.h>
#include <string>

using namespace std;

extern "C" {
  #include <string.h>
  #include <alloca.h>
  #include "http.h"
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
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
