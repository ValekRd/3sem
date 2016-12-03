cmake_minimum_required(VERSION 3.6)
project(server_client)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

set(SOURCE_FILES server.c )

add_executable(server ${SOURCE_FILES})
add_executable(client ${SOURCE_FILES})