cmake_minimum_required(VERSION 2.8)

add_executable(1px-wayland-01 1px-wayland-01.cpp)

set(CMAKE_BUILD_TYPE Debug)

target_link_libraries(1px-wayland-01 wayland-client)
