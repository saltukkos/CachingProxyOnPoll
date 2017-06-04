cmake_minimum_required(VERSION 3.5)
project(ProxyOnPoll)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g3 -fno-inline -O0")

include_directories(include)
set(SOURCE_FILES src/main.cpp src/Request.cpp src/CacheEntry.cpp src/Client.cpp src/HTTPProxy.cpp src/Acceptor.cpp src/PollStorage.cpp src/CacheManager.cpp src/Downloader.cpp)
add_executable(ProxyOnPoll ${SOURCE_FILES})
