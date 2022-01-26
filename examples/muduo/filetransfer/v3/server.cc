#include "server.h"

#include <cstdio>
#include <functional>

#include "muduo/base/CurrentThread.h"
#include "muduo/base/Logging.h"

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

const int kBufSize = 1 * 1024;

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

FileTransferServer::~FileTransferServer() { delete server_; }

void FileTransferServer::onConnection(const TcpConnectionPtr& conn) {
  if (conn->connected()) {
    FILE* fp = ::fopen(filename_.c_str(), "r");
    if (!fp) {
      conn->shutdown();
      LOG_INFO << "no such file";
      return;
    }

    FilePtr filePtr(fp, [](FILE* fp) { ::fclose(fp); });

    conn->setContext(filePtr);
    char content[kBufSize];
    size_t nread;
    if ((nread = ::fread(content, 1, sizeof content, filePtr.get())) > 0) {
      conn->send(content, nread);
    }
  } else {
    // 当TcpConnection 析构时, fp 会被close
  }
}

void FileTransferServer::onWriteComplete(const TcpConnectionPtr& conn) {
  FilePtr fp = boost::any_cast<FilePtr>(conn->getContext());
  char buf[kBufSize];
  size_t nread;
  if ((nread = ::fread(buf, 1, sizeof buf, fp.get())) > 0) {
    muduo::CurrentThread::sleepUsec(1000 * 100);
    conn->send(buf, nread);
  } else {
    // 当TcpConnction 析构时, fp会被 close
  }
}
