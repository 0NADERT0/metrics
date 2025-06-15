######################################################
cmake_minimum_required(VERSION 3.20)
project(Metrics)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Threads REQUIRED)

add_executable(server
    src/server.cc
    src/metrics.cc
)

target_include_directories(server
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(server
    PRIVATE
    Threads::Threads
    ${CMAKE_THREAD_LIBS_INIT}
)

add_executable(client src/client.cc)

target_include_directories(client
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)