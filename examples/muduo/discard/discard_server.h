#include <functional>

#include "muduo/base/Timestamp.h"
#include "muduo/net/Buffer.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/TcpServer.h"

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

using muduo::Timestamp;
using muduo::net::Buffer;
using muduo::net::EventLoop;
using muduo::net::InetAddress;
using muduo::net::TcpConnectionPtr;
using muduo::net::TcpServer;

class DiscardServer {
 public:
  DiscardServer(EventLoop* loop, const InetAddress& listenAddr)
      : server_(new TcpServer(loop, listenAddr, "DiscardServer")) {
    server_->setMessageCallback(
        std::bind(&DiscardServer::onMessage, this, _1, _2, _3));
  }

  ~DiscardServer() { delete server_; }

  void start() { server_->start(); }

  void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp ts);

 private:
  TcpServer* server_;
};