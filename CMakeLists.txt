cmake_minimum_required(VERSION 3.7.2)

project(klamath)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto -fuse-linker-plugin")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -flto -fuse-linker-plugin")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)

find_package(SDL2 REQUIRED)
if(NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 library not found")
endif(NOT SDL2_FOUND)

add_executable(${PROJECT_NAME}
  third_party/miniz.c

  src/utils/io.cpp
  src/utils/zlib.cpp
  src/utils/sdl.cpp
  
  src/formats/dat2_reader.cpp
  src/formats/aaf_reader.cpp
  src/formats/frm.cpp
  src/formats/frm_reader.cpp
  src/formats/pal_reader.cpp
  src/formats/map_reader.cpp
  
  src/cmds/dat2extract.cpp
  src/cmds/aafprint.cpp
  src/cmds/paldump.cpp
  src/cmds/frmshow.cpp
  src/cmds/frmheader.cpp
  src/cmds/mapheader.cpp
  
  src/klamath.cpp
  )

target_include_directories(${PROJECT_NAME} PUBLIC
  ./
  ${SDL2_INCLUDE_DIR}
  )

target_link_libraries(${PROJECT_NAME} PUBLIC
  ${SDL2_LIBRARY}
)
