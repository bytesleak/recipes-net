#include <cstdio>
#include <memory>
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
  FileTransferServer(EventLoop* loop, const InetAddress& listenAddr, const char* filename);
  ~FileTransferServer();
  void onConnection(const TcpConnectionPtr& conn);
  void onWriteComplete(const TcpConnectionPtr& conn);
  void start() { server_->start(); }

 private:
  typedef std::shared_ptr<FILE> FilePtr;
  TcpServer* server_;
  std::string filename_;
};