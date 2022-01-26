#include <memory>
#include <string>

#include "muduo/base/Timestamp.h";
#include "muduo/net/Buffer.h"
#include "muduo/net/TcpConnection.h"
#include "muduo/net/TcpServer.h"

using muduo::Timestamp;
using muduo::net::Buffer;
using muduo::net::EventLoop;
using muduo::net::InetAddress;
using muduo::net::TcpConnectionPtr;
using muduo::net::TcpServer;
class EchoServer {
 public:
  typedef std::function<void(std::shared_ptr<std::string> msgPtr)>
      EchoCapabilitiy;

  EchoServer(EventLoop* loop, const InetAddress& listenAddr);

  EchoServer(EventLoop* loop, const InetAddress& listenAddr,
             std::initializer_list<const EchoCapabilitiy&> capabilities);

  ~EchoServer() { delete server_; }

  void Start();

  void OnConnection(const TcpConnectionPtr& conn);

  void OnMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp ts);

 private:
  TcpServer* server_;
};