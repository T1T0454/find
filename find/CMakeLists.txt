cmake_minimum_required(VERSION 3.15)
project(hw05 C)

set(CMAKE_C_STANDARD 99)

add_executable(hw05 main.c sort.c sort.h parseArgs.c parseArgs.h)