cmake_minimum_required(VERSION 3.9)

project(klamath-utils)


add_executable(${PROJECT_NAME} src/klamath_utils.cpp src/Dat2Extract.cpp src/Dat2Extract.h src/FrmPrintHeader.cpp src/FrmPrintHeader.h src/AafPrintGlyphs.cpp src/AafPrintGlyphs.h src/PalDump.cpp src/PalDump.h)

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

target_link_libraries(${PROJECT_NAME} libklamath-io-cpp)
