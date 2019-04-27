project(klamath)

find_package(SDL2 REQUIRED)
if(NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 library not found")
endif(NOT SDL2_FOUND)

find_package(ZLIB REQUIRED)
if(NOT ZLIB_FOUND)
  message(FATAL_ERROR "zlib library not found: probably not installed (e.g. try `apt install zlib-dev`)")
endif()

add_executable(${PROJECT_NAME}
  src/utils/byteorder_buf.cpp
  src/utils/byteorder_ios.cpp
  src/utils/cursor.cpp
  src/utils/zlib.cpp
  src/utils/sdl.cpp
  src/utils/color.cpp
  
  src/parsers/dat2.cpp
  src/parsers/aaf.cpp
  src/parsers/frm.cpp
  src/parsers/pal.cpp
  
  src/commands/dat2extract.cpp
  src/commands/aafprint.cpp
  src/commands/paldump.cpp
  src/commands/frmshow.cpp
  src/commands/frmheader.cpp
  src/commands/frmdump.cpp
  
  src/klamath.cpp
  )

target_include_directories(${PROJECT_NAME} PUBLIC
  ./
  ZLIB_INCLUDE_DIRS
  ${SDL2_INCLUDE_DIR}
  )

target_link_libraries(${PROJECT_NAME} PUBLIC
  ${ZLIB_LIBRARIES}
  ${SDL2_LIBRARY}
)
