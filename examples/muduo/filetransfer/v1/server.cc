#include "server.h"

#include <cstdio>
#include <functional>

#include "muduo/base/Logging.h"

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

FileTransferServer::FileTransferServer(EventLoop* loop,
                                       const InetAddress& listenAddr)
    : server_(new TcpServer(loop, listenAddr, "FileTransferServer")) {
  server_->setMessageCallback(
      std::bind(&FileTransferServer::onMessage, this, _1, _2, _3));
}

void FileTransferServer::onMessage(const TcpConnectionPtr& conn, Buffer* buf,
                                   Timestamp ts) {
  const char* eol = buf->findEOL();
  if (!eol) {
    conn->send("file invalid");
    conn->shutdown();
    return;
  }

  // copy
  std::string filename(buf->peek(), eol);

  std::string content = readFile(filename.c_str());
  if (content.size() == 0) {
    conn->send("file read error");
    conn->shutdown();
    return;
  }

  LOG_INFO << "send " << filename;

  conn->send(content);
}

std::string FileTransferServer::readFile(const char* filename) {
  std::string content;

  // FIXME 共享文件描述符
  FILE* fp = ::fopen(filename, "r");
  if (fp == nullptr) {
    return content;
  }

  // FIXME 每次读取文件都拷贝整个
  size_t kBufSize = 1024 * 1024;
  char buf[kBufSize];
  size_t nread;

  while ((nread = ::fread(buf, 1, kBufSize, fp)) > 0) {
    content.append(buf, nread);
  }

  ::fclose(fp);

  return content;
}
