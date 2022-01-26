#include "discard_server.h"

#include <string>

#include "muduo/base/Logging.h"

void DiscardServer::onMessage(const TcpConnectionPtr& conn, Buffer* buf,
                              Timestamp ts) {
  std::string msg(buf->retrieveAllAsString());
  LOG_INFO << "discards " << msg.size() << " bytes received at "
           << ts.toString();
}