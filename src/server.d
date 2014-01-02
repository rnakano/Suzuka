import std.socket;

class Server {
  private ushort port;
  private void delegate(Socket) handler;
  private static const uint CONNECTION_QUEUE_SIZE = 10;

  this(ushort port) {
    this.port = port;
  }

  void setRequestHandler(void delegate(Socket) handler) {
    this.handler = handler;
  }

  void start() {
    Socket listener = new TcpSocket();
    assert(listener.isAlive);
    listener.blocking = true;
    listener.setOption(SocketOptionLevel.SOCKET, SocketOption.REUSEADDR, 1);
    listener.bind(new InternetAddress(port));
    listener.listen(CONNECTION_QUEUE_SIZE);

    while(true) {
      Socket sock = listener.accept();
      handler(sock);
    }
  }
}
  
