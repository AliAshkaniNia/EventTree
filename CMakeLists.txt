cmake_minimum_required(VERSION 3.15) 
project(EventTree)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/utils.cmake)

add_executable(EventTree 
    src/main.cpp
    src/eventHub/EventppHub.cpp
    src/eventProducers/Ahriman.cpp
    src/eventProducers/Anahita.cpp
)


add_common_compile_options(EventTree)
add_clang_format(EventTree TRUE )
add_clang_tidy(EventTree FALSE)

setup_library(eventpp 
    https://github.com/wqking/eventpp 
    d2db8af2a46c79f8dc75759019fba487948e9442 # v0.1.3
)

target_include_directories(EventTree PRIVATE ${eventpp_SOURCE_DIR}/include)
target_include_directories(EventTree PRIVATE include/)
