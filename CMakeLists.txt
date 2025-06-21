cmake_minimum_required(VERSION 3.16)

project(gsh C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SRC_FILES src/*.c)

set(MAIN_FILE main.c)

add_executable(${PROJECT_NAME} ${MAIN_FILE} ${SRC_FILES})
