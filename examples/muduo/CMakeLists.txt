include_directories(${PROJECT_SOURCE_DIR}/third_party/muduo/ ${PROJECT_SOURCE_DIR}/third_party/muduo/)

# compiler muduo_base lib
set(base_SRCS
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/base/AsyncLogging.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/base/Condition.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/base/CountDownLatch.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/base/CurrentThread.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/base/Date.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/base/Exception.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/base/FileUtil.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/base/LogFile.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/base/Logging.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/base/LogStream.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/base/ProcessInfo.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/base/Timestamp.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/base/Thread.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/base/ThreadPool.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/base/TimeZone.cc
)

add_library(muduo_base ${base_SRCS})
target_link_libraries(muduo_base pthread rt)

# compiler muduo_net lib
include(CheckFunctionExists)

check_function_exists(accept4 HAVE_ACCEPT4)
if(NOT HAVE_ACCEPT4)
  set_source_files_properties(SocketsOps.cc PROPERTIES COMPILE_FLAGS "-DNO_ACCEPT4")
endif()

set(net_SRCS
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/net/Acceptor.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/net/Buffer.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/net/Channel.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/net/Connector.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/net/EventLoop.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/net/EventLoopThread.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/net/EventLoopThreadPool.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/net/InetAddress.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/net/Poller.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/net/poller/DefaultPoller.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/net/poller/EPollPoller.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/net/poller/PollPoller.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/net/Socket.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/net/SocketsOps.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/net/TcpClient.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/net/TcpConnection.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/net/TcpServer.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/net/Timer.cc
    ${PROJECT_SOURCE_DIR}/third_party/muduo/muduo/net/TimerQueue.cc
)

add_library(muduo_net ${net_SRCS})

# compiler boost

target_link_libraries(muduo_net muduo_base)
target_link_libraries(muduo_net Boost::boost)

# compiler examples
add_executable(echo echo/main.cc echo/echo_server.cc)
target_link_libraries(echo muduo_net)

add_executable(discard discard/main.cc discard/discard_server.cc)
target_link_libraries(discard muduo_net)

add_executable(daytime daytime/main.cc daytime/daytime_server.cc)
target_link_libraries(daytime muduo_net)

add_executable(chargen chargen/main.cc chargen/chargen_server.cc)
target_link_libraries(chargen muduo_net)

add_executable(filetransfer_v1 filetransfer/v1/main.cc filetransfer/v1/server.cc)
target_link_libraries(filetransfer_v1 muduo_net)

add_executable(filetransfer_v2 filetransfer/v2/main.cc filetransfer/v2/server.cc)
target_link_libraries(filetransfer_v2 muduo_net)

add_executable(filetransfer_v3 filetransfer/v3/main.cc filetransfer/v3/server.cc)
target_link_libraries(filetransfer_v3 muduo_net)

add_executable(filetransfer_v4 filetransfer/v4/main.cc filetransfer/v4/server.cc)
target_link_libraries(filetransfer_v4 muduo_net)
