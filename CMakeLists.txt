cmake_minimum_required(VERSION 4.1)
project(PP)

set(CMAKE_CXX_STANDARD 20)

find_package(Threads REQUIRED)

enable_testing()

add_subdirectory(./lw1)
add_subdirectory(./lw2)
add_subdirectory(./lw3)
