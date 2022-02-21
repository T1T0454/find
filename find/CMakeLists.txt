cmake_minimum_required(VERSION 3.15)
project(custom_find C)

set(CMAKE_C_STANDARD 99)

add_executable(custom_find main.c sort.c sort.h parseArgs.c parseArgs.h)
