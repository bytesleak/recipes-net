#include "echo_server.h"
#include "muduo/net/EventLoop.h"
#include "muduo/base/Logging.h"
#include "unistd.h"
int main()
{
    LOG_INFO << "pid = " << getpid();   
    EventLoop loop;
    InetAddress listenAddr(8000);
    EchoServer server(&loop, listenAddr);
    server.Start();
    loop.loop();
    return 0;
}