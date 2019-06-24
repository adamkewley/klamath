project(klamath)

find_package(SDL2 REQUIRED)
if(NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 library not found")
endif(NOT SDL2_FOUND)

add_executable(${PROJECT_NAME}
  third_party/miniz.c
  
  src/utils/byteorder_buf.cpp
  src/utils/byteorder_ios.cpp
  src/utils/cursor.cpp
  src/utils/zlib.cpp
  src/utils/sdl.cpp
  
  src/formats/dat2_reader.cpp
  src/formats/aaf_reader.cpp
  src/formats/frm.cpp
  src/formats/frm_reader.cpp
  src/formats/pal_reader.cpp
  
  src/commands/dat2extract.cpp
  src/commands/aafprint.cpp
  src/commands/paldump.cpp
  src/commands/frmshow.cpp
  src/commands/frmheader.cpp
  
  src/klamath.cpp
  )

target_include_directories(${PROJECT_NAME} PUBLIC
  ./
  ${SDL2_INCLUDE_DIR}
  )

target_link_libraries(${PROJECT_NAME} PUBLIC
  ${SDL2_LIBRARY}
)
