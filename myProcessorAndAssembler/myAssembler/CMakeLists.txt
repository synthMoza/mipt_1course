cmake_minimum_required(VERSION 3.15)
project(myAssembler C)

set(CMAKE_C_STANDARD 99)

add_executable(myAssembler assembly.c assembly.h def_com.h def_reg.h assembly_macro.h)