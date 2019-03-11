project(libklamath)

add_library(${PROJECT_NAME} SHARED
  ../vendor/miniz/miniz.c
  
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
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  POSITION_INDEPENDENT_CODE ON
)
