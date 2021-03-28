cmake_minimum_required(VERSION 3.15)
project(myProcessor C)

set(CMAKE_C_STANDARD 99)

add_executable(myProcessor cpu.c cpu.h cpu_macro.h)

target_link_libraries(myProcessor m)