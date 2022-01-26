#include <cstdio>

#include "muduo/base/Timestamp.h"
#include "muduo/net/TcpServer.h"

using muduo::Timestamp;
using muduo::net::Buffer;
using muduo::net::EventLoop;
using muduo::net::InetAddress;
using muduo::net::TcpConnectionPtr;
using muduo::net::TcpServer;


class FileTransferServer {
 public:
  FileTransferServer(EventLoop* loop, const InetAddress& listenAddr);

  void onConnection(const TcpConnectionPtr& conn);
  void onMessage(const TcpConnectionPtr& ptr, Buffer* buf, Timestamp ts);
  void onWriteComplete(const TcpConnectionPtr& conn);
  void start() { server_->start(); }

 private:
  TcpServer* server_;
};