project(libklamath-io-cpp)

add_library(${PROJECT_NAME} SHARED
        src/aaf/AafParser.cpp
        src/dat2/Dat2Parser.cpp
        src/frm/FrmParser.cpp
        src/pal/PalParser.cpp
        src/utils/Stream.cpp
        src/utils/MemoryStream.cpp)

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