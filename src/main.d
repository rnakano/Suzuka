import std.socket, std.stdio, std.file, std.path, std.uni, std.conv;
import http, server;

const static string NL = "\r\n";

pure const string contentType(string ext) {
  switch(ext.toLower()) {
  case "jpg", "jpeg":
    return "image/jpeg";
  case "png":
    return "image/png";
  case "gif":
    return "image/gif";
  default:
    return "text/html";
  }
}

unittest {
  assert( contentType("JPG") == "image/jpeg" );
  assert( contentType("hoge") == "text/html" );
}

void sendFile(Socket sock, string path)
{
  string response = "HTTP/1.1 200 OK" ~ NL;
  string type = contentType(extension(path)[1 .. $]);
  auto file = File(path, "rb");
  auto size = getSize(path);
  auto last = timeLastModified(path);
  response ~= "Content-Type: " ~ type ~ NL;
  response ~= "Last-Modified: " ~ last.toString() ~ NL;
  response ~= "Cotent-Length: " ~ to!string(size) ~ NL;
  response ~= "Connection: close" ~ NL;
  response ~= "Server: Suzuka" ~ NL;
  response ~= NL;

  sock.send(response);
  foreach(ubyte[] buffer; file.byChunk(4096)) {
    sock.send(buffer);
  }
}

void send404(Socket sock)
{
  sock.send("HTTP/1.1 404 Not Found" ~ NL ~ NL);
}

void main(string[] args)
{
  if(args.length == 2 && args[1] == "test") return;
  Server server = new Server(8080);
  HttpRequestParser parser = new HttpRequestParser();
  const string root = "./public";
  server.setRequestHandler( (Socket sock) {
      char[256] buff;
      auto read = sock.receive(buff);
      if(Socket.ERROR == read) {
        writeln("Connection error.");
        sock.close();
        return;
      }
      HttpRequestHeader header = parser.parse(buff[0 .. read].idup);
      if(header is null) {
        writeln("Parse error.");
        sock.close();
        return;
      }
      writeln("Path: " ~ header.path() ~ " requested.");
      string path = root ~ header.path();
      if(exists(path)) {
        sendFile(sock, path);
      } else {
        send404(sock);
      }
      sock.close();
  });
  server.start();
}
