cmake_minimum_required(VERSION 3.5)

project(test_uvclientserver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

add_executable(${PROJECT_NAME}
    unittest.cpp
    utility/capture_stdout.h
    ../src/server/server.cpp
    ../src/server/server.h
    ../src/server/server_client.cpp
    ../src/server/server_client.h
    ../src/client/client.cpp
    ../src/client/client.h
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ./ ../libs
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE TESTING
)

target_link_libraries(${PROJECT_NAME}
    uv_network
)
