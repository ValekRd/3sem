cmake_minimum_required(VERSION 3.6)
project(Posuda)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

set(SOURCE_FILES main.c dishes.txt washer.txt wiper.txt key)

add_executable(main ${SOURCE_FILES})