project(klamath-utils)


find_package(SDL2 REQUIRED)
if(NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 library not found")
endif(NOT SDL2_FOUND)
include_directories(${SDL2_INCLUDE_DIR})


add_executable(${PROJECT_NAME}
  src/dat2extract.cpp
  src/aafprint.cpp
  src/paldump.cpp
  src/frmshow.cpp
  src/sdl.cpp
  src/frmheader.cpp

  src/klamath_utils.cpp
  )

target_include_directories(${PROJECT_NAME} PUBLIC
  ./
)

target_link_libraries(${PROJECT_NAME}
  libklamath
  ${SDL2_LIBRARY}
)
