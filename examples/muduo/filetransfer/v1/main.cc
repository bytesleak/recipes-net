#include "muduo/net/EventLoop.h"
#include "server.h"

int main() {
  EventLoop loop;
  InetAddress listenAddr(2016);
  FileTransferServer server(&loop, listenAddr);
  server.start();
  loop.loop();
}