cmake_minimum_required(VERSION 3.6)
project(Task4)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

set(SOURCE_FILES 1.c key)

add_executable(1 ${SOURCE_FILES})
