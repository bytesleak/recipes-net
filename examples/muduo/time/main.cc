#include "daytime_server.h"

int main() {
  EventLoop loop;
  InetAddress listen(2013);

  DaytimeServer server(&loop, listen);
  server.start();
  loop.loop();
  return 0;
}