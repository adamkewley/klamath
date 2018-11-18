cmake_minimum_required(VERSION 3.9)

project(klamath-utils)


find_package(SDL2 REQUIRED)
if(NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 library not found")
endif(NOT SDL2_FOUND)
include_directories(${SDL2_INCLUDE_DIR})


add_executable(${PROJECT_NAME}
        src/klamath_utils.cpp
        src/Dat2Extract.cpp
        src/FrmPrintHeader.cpp
        src/AafPrintGlyphs.cpp
        src/PalDump.cpp
        src/FrmDumpFirstFrame.cpp)

target_link_libraries(${PROJECT_NAME}
        libklamath-io-cpp
        ${SDL2_LIBRARY})

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)
