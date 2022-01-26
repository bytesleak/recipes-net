#include "echo_server.h"

#include <cassert>
#include <functional>
#include <iostream>
#include <string>

#include "muduo/base/Logging.h"
#include "muduo/net/InetAddress.h"

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

using muduo::net::InetAddress;

EchoServer::EchoServer(EventLoop* loop, const InetAddress& listenAddr)
    : server_(new TcpServer(loop, listenAddr, "EchoServer")) {
  server_->setConnectionCallback(
      std::bind(&EchoServer::OnConnection, this, _1));
  server_->setMessageCallback(
      std::bind(&EchoServer::OnMessage, this, _1, _2, _3));
}

void EchoServer::Start() { server_->start(); }

void EchoServer::OnConnection(const TcpConnectionPtr& conn) {
  const InetAddress& peer = conn->peerAddress();
  if (conn->connected()) {
    LOG_INFO << " connection";
  } else {
    LOG_INFO << "disconnected";
  }
}

void EchoServer::OnMessage(const TcpConnectionPtr& conn, Buffer* buf,
                           Timestamp ts) {
  std::string msg = buf->retrieveAllAsString();
  conn->send(msg.c_str(), msg.size());
}