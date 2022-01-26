#include "discard_server.h"

int main() {
  EventLoop loop;
  InetAddress listen(8001);

  DiscardServer server(&loop, listen);
  server.start();
  loop.loop();
  return 0;
}