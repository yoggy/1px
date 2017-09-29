cmake_minimum_required(VERSION 2.8)

add_executable(1px-cogl-01 1px-cogl-01.c)

set(CMAKE_BUILD_TYPE Debug)

find_package(PkgConfig REQUIRED)

# gcc 1px-cogl-01 -DCOGL_ENABLE_EXPERIMENTAL_API -I/usr/include/cogl `pkg-config --cflags glib-2.0` `pkg-config --libs glib-2.0`  -lcogl
add_definitions(-DCOGL_ENABLE_EXPERIMENTAL_API)

include_directories( /usr/include/cogl /usr/include/glib-2.0 /usr/lib/glib-2.0/include )
target_link_libraries(1px-cogl-01 cogl glib-2.0 rt)
