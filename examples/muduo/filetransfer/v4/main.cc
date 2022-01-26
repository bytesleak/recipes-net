#include <cstdio>

#include "muduo/net/EventLoop.h"
#include "server.h"

int main(int argc, const char* argv[]) {
  if (argc != 2) {
    std::printf("please provider filename\n");
    exit(1);
  }

  EventLoop loop;
  InetAddress listenAddr(2016);
  FileTransferServer server(&loop, listenAddr, argv[1]);
  server.initFile();
  server.start();
  loop.loop();
}