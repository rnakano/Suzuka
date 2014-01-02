import std.socket, std.stdio;
import http, server;

void main(string[] args)
{
  if(args.length == 2 && args[1] == "test") return;
  Server server = new Server(8080);
  HttpRequestParser parser = new HttpRequestParser();
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
      sock.close();
  });
  server.start();
}
