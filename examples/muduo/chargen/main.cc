#include "chargen_server.h"

int main() {
  EventLoop loop;
  InetAddress listenAddr(2015);

  ChargenServer server(&loop, listenAddr);
  server.start();
  loop.loop();
  return 0;
}