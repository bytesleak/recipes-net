#include "server.h"

#include <cstdio>
#include <functional>

#include "muduo/base/CurrentThread.h"
#include "muduo/base/Logging.h"

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

const int kBufSize = 1*1024;

FileTransferServer::FileTransferServer(EventLoop* loop,
                                       const InetAddress& listenAddr)
    : server_(new TcpServer(loop, listenAddr, "FileTransferServer")) {
  server_->setMessageCallback(
      std::bind(&FileTransferServer::onMessage, this, _1, _2, _3));

  server_->setWriteCompleteCallback(
      std::bind(&FileTransferServer::onWriteComplete, this, _1));
}

void FileTransferServer::onConnection(const TcpConnectionPtr& conn) {
  if (!conn->connected() && !conn->getContext().empty()) {
    FILE* fp = boost::any_cast<FILE*>(conn->getContext());
    if (fp) {
      ::fclose(fp);
    }
  } 
}

void FileTransferServer::onMessage(const TcpConnectionPtr& conn, Buffer* buf,
                                   Timestamp ts) {
  const char* eol = buf->findEOL();
  if (!eol) {
    conn->shutdown();
    LOG_INFO << "filename invalid";
    return;
  }
  std::string filename(buf->peek(), eol);
  LOG_INFO << "start transfer " << filename;

  // FIXME to share fp
  FILE* fp = ::fopen(filename.c_str(), "r");
  if (!fp) {
    conn->shutdown();
    LOG_INFO << "no such file";
    return;
  }

  char content[kBufSize];
  conn->setContext(fp);
  size_t nread;
  if ((nread = ::fread(content, 1, sizeof content, fp)) > 0) {
    conn->send(content, nread);
  }
}

void FileTransferServer::onWriteComplete(const TcpConnectionPtr& conn) {
  FILE* fp = boost::any_cast<FILE*>(conn->getContext());
  char buf[kBufSize];
  size_t nread;
  if ((nread = ::fread(buf, 1, sizeof buf, fp)) > 0) {
    // muduo::CurrentThread::sleepUsec(1000*1000);
    conn->send(buf, nread);
  } else {
    ::fclose(fp);
    fp = NULL;
    conn->setContext(fp);
    LOG_INFO << "transfer done.";
  }
}
