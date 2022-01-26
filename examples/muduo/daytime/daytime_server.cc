#include "daytime_server.h"

#include <string>

void DaytimeServer::onConnection(const TcpConnectionPtr& conn) {
  if (conn->connected()) {
    conn->send(Timestamp::now().toFormattedString() + "\n");
    conn->shutdown();
  }
}