cmake_minimum_required(VERSION 3.15) 
project(EventTree)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/utils.cmake)

add_executable(EventTree src/main.cpp)


add_common_compile_options(EventTree)
add_clang_format(EventTree TRUE )
add_clang_tidy(EventTree FALSE)