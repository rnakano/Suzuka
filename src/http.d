import std.string, std.array;

class HttpRequestHeader {
  private string _path;

  this(string path) {
    this._path = path;
  }

  public string path(){
    return this._path;
  }
};

class HttpRequestParser {
  public HttpRequestHeader parse(string headerText) {
    auto lines = headerText.splitLines();
    if(lines.empty) return null;
    string path = parse_path(lines[0]);
    if(path !is null) {
      return new HttpRequestHeader(path);
    } else {
      return null;
    }
  }

  private string parse_path(string line) {
    auto terms = line.split(" ");
    if(terms.length != 3) return null;
    if(terms[0] != "GET") return null;
    if(!(terms[2] == "HTTP/1.1" || terms[2] == "HTTP/1.0")) return null;
    return terms[1];
  }
}

unittest {
  auto parser = new HttpRequestParser();
  auto header = parser.parse(
"GET /img/hoge.jpg HTTP/1.1
User-Agent: curl/7.24.0 (x86_64-apple-darwin12.0) libcurl/7.24.0 OpenSSL/0.9.8y zlib/1.2.5
Host: localhost:8080
Accept: */*

");

  assert( header.path() == "/img/hoge.jpg" );

  assert( parser.parse("hoge") is null);
}
