cmake_minimum_required(VERSION 3.9)

project(libklamath-io-cpp)

add_library(${PROJECT_NAME} SHARED
        src/dat2/Dat2FileViewer.cpp
        src/dat2/Dat2Parser.cpp
        src/lib/Mmap.cpp
        src/lib/ZlibHelpers.cpp
        src/aaf/AafParser.cpp)

find_package( ZLIB REQUIRED )
if ( ZLIB_FOUND )
    include_directories( ${ZLIB_INCLUDE_DIRS} )
    target_link_libraries( ${PROJECT_NAME} ${ZLIB_LIBRARIES} )
endif( ZLIB_FOUND )

target_include_directories(${PROJECT_NAME}
        PUBLIC
        src)


set_target_properties(${PROJECT_NAME} PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)