cmake_minimum_required(VERSION 3.6)
project(stack__queue)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

set(SOURCE_FILES queue.c )

add_executable(queue ${SOURCE_FILES})
add_executable(stack ${SOURCE_FILES})
