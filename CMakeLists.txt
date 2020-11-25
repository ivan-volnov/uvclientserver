cmake_minimum_required(VERSION 3.5)

project(uvclientserver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

add_subdirectory(libs/libuv)
add_subdirectory(test)

add_library(uv_network STATIC
    src/utility/stack.h
    src/uv_network/parser.h
    src/uv_network/protocol.cpp
    src/uv_network/protocol.h
    src/uv_network/uv_buffers.cpp
    src/uv_network/uv_buffers.h
    src/uv_network/uv_network.cpp
    src/uv_network/uv_network.h
    src/uv_network/uv_tcp_handle.cpp
    src/uv_network/uv_tcp_handle.h
)

target_link_libraries(uv_network
    uv_a
)

target_include_directories(uv_network
    PRIVATE ./
    PUBLIC src
)



add_executable(uv_server
    src/server/main.cpp
    src/server/server.cpp
    src/server/server.h
    src/server/server_client.cpp
    src/server/server_client.h
)

target_include_directories(uv_server
    PRIVATE ./
)

target_link_libraries(uv_server
    uv_network
)



add_executable(uv_client
    src/client/client.cpp
    src/client/client.h
    src/client/main.cpp
)

target_include_directories(uv_client
    PRIVATE ./
)

target_link_libraries(uv_client
    uv_network
)
