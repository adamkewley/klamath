cmake_minimum_required(VERSION 3.9)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)

add_subdirectory(libklamath-io-cpp)
add_subdirectory(klamath-utils)
