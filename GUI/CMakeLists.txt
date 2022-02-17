cmake_minimum_required (VERSION 3.10)
project(helloBrain)

set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})

add_definitions(${GTK4_CFLAGS_OTHER})

add_executable(helloBrain src/main.cpp)

target_link_libraries(helloBrain ${GTK4_LIBRARIES})