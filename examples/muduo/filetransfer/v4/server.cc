#include "server.h"

#include <fcntl.h>
#include <unistd.h>

#include <cassert>
#include <cerrno>
#include <functional>

#include "muduo/base/CurrentThread.h"
#include "muduo/base/Logging.h"

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

const int kBufSize = 10 * 1024;

FileTransferServer::FileTransferServer(EventLoop* loop,
                                       const InetAddress& listenAddr,
                                       const char* filename)
    : server_(new TcpServer(loop, listenAddr, "FileTransferServer")),
      filename_(std::string(filename)) {
  server_->setConnectionCallback(
      std::bind(&FileTransferServer::onConnection, this, _1));

  server_->setWriteCompleteCallback(
      std::bind(&FileTransferServer::onWriteComplete, this, _1));
}

FileTransferServer::~FileTransferServer() {
  delete server_;
  ::close(fd_);
}

// 二段式构造, 防止构造函数异常
void FileTransferServer::initFile() {
  fd_ = ::open(filename_.c_str(), O_RDONLY);
  if (fd_ == -1) {
    perror("open");
    abort();
  }
}

void FileTransferServer::start() { server_->start(); }

void FileTransferServer::onConnection(const TcpConnectionPtr& conn) {
  if (conn->connected()) {
    char content[kBufSize];
    size_t nread;
    if ((nread = ::pread(fd_, content, sizeof content, 0)) > 0) {
      conn->setContext(nread);
      conn->send(content, nread);
    }
  }
}

void FileTransferServer::onWriteComplete(const TcpConnectionPtr& conn) {
  size_t offset = boost::any_cast<size_t>(conn->getContext());
  char buf[kBufSize];
  size_t nread;
  if ((nread = ::pread(fd_, buf, sizeof buf, offset)) > 0) {
    // muduo::CurrentThread::sleepUsec(1000 * 1000);
    conn->send(buf, nread);
    conn->setContext(offset + nread);
  } else {
    conn->setContext(0);
    LOG_INFO << "transfer done.";
  }
}
