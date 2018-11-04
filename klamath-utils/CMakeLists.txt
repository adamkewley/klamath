cmake_minimum_required(VERSION 3.9)

project(klamath-utils)


add_executable(${PROJECT_NAME} src/klamath_utils.cpp src/dat2.cpp src/dat2.h)

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

target_link_libraries(${PROJECT_NAME} libklamath-io-cpp)
