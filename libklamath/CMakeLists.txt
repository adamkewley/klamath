project(libklamath)

find_package(ZLIB REQUIRED)
if(NOT ZLIB_FOUND)
  message(FATAL_ERROR "zlib library not found: probably not installed (e.g. try `apt install zlib-dev`)")
endif()

add_library(${PROJECT_NAME} SHARED
  src/ioutils.cpp
  
  src/dat2.cpp
  src/aaf.cpp
  src/frm.cpp
  src/pal.cpp
  src/zlib.cpp
)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -flto")

target_include_directories(${PROJECT_NAME} PUBLIC
  include/
  ./
  ../
  ZLIB_INCLUDE_DIRS
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  ${ZLIB_LIBRARIES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  POSITION_INDEPENDENT_CODE ON
)
