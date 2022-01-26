#include <functional>

#include "muduo/base/Timestamp.h"
#include "muduo/net/Buffer.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/TcpServer.h"

using std::placeholders::_1;

using muduo::Timestamp;
using muduo::net::Buffer;
using muduo::net::EventLoop;
using muduo::net::InetAddress;
using muduo::net::TcpConnectionPtr;
using muduo::net::TcpServer;

class DaytimeServer {
 public:
  DaytimeServer(EventLoop* loop, const InetAddress& listenAddr)
      : server_(new TcpServer(loop, listenAddr, "DaytimeServer")) {
    server_->setConnectionCallback(
        std::bind(&DaytimeServer::onConnection, this, _1));
  }

  ~DaytimeServer() { delete server_; }

  void start() { server_->start(); }

  void onConnection(const TcpConnectionPtr& conn);

 private:
  TcpServer* server_;
};