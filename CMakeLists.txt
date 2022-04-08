cmake_minimum_required(VERSION 3.21)
project(linux_system_API C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(linux_system_API 1-1stat.c)
