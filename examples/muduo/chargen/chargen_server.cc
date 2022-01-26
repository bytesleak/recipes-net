#include "chargen_server.h"

#include "muduo/base/Logging.h"

ChargenServer::ChargenServer(EventLoop* loop, const InetAddress& listenAddr)
    : server_(new TcpServer(loop, listenAddr, "ChargenServer")) {
  std::string line;
  for (int i = 33; i < 127; ++i) {
    line.push_back(char(i));
  }
  line += line;

  for (size_t i = 0; i < 127 - 33; ++i) {
    message_ += line.substr(i, 72) + '\n';
  }

  server_->setConnectionCallback(
      std::bind(&ChargenServer::onConnection, this, _1));

  server_->setMessageCallback(
      std::bind(&ChargenServer::onMessage, this, _1, _2, _3));

  server_->setWriteCompleteCallback(
      std::bind(&ChargenServer::onWriteComplete, this, _1));
}

void ChargenServer::onConnection(const TcpConnectionPtr& conn) {
  // 避免延迟, 我们需要立即发送message到client
  conn->setTcpNoDelay(true);
  conn->send(message_);
}

void ChargenServer::onMessage(const TcpConnectionPtr& conn, Buffer* buf,
                              Timestamp time) {
  std::string msg(buf->retrieveAllAsString());
  LOG_INFO << conn->name() << " discard " << msg.size() << " bytes";
}

void ChargenServer::onWriteComplete(const TcpConnectionPtr& conn) {
  // 当client可以接受消息时, 我们在发送
  conn->send(message_);
}