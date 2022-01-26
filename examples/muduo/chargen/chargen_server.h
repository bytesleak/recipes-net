#include <functional>
#include <memory>
#include <string>

#include "muduo/base/Timestamp.h"
#include "muduo/net/Buffer.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/TcpConnection.h"
#include "muduo/net/TcpServer.h"

using muduo::Timestamp;
using muduo::net::Buffer;
using muduo::net::EventLoop;
using muduo::net::InetAddress;
using muduo::net::TcpConnectionPtr;
using muduo::net::TcpServer;
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

class ChargenServer {
 public:
  ChargenServer(EventLoop* loop, const InetAddress& listenAddr);

  ~ChargenServer() { delete server_; }

  void onConnection(const TcpConnectionPtr& conn);

  void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp ts);

  void onWriteComplete(const TcpConnectionPtr& conn);

  void start() { server_->start(); }

 private:
  TcpServer* server_;
  std::string message_;
};