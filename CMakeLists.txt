
cmake_minimum_required(VERSION 3.16)

project(oppenheimer-simulator LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED IMPORTED_TARGET SDL2_image)
pkg_check_modules(SDL2_MIXER REQUIRED IMPORTED_TARGET SDL2_mixer)

file(GLOB SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.c"
)

list(FILTER SOURCES EXCLUDE REGEX "/config-test\\.c$")

add_executable(oppenheimer-simulator ${SOURCES})

target_link_libraries(oppenheimer-simulator PRIVATE
    PkgConfig::SDL2
    PkgConfig::SDL2_IMAGE
    PkgConfig::SDL2_MIXER
)

target_compile_options(oppenheimer-simulator PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -g
    -O1
    -fno-omit-frame-pointer
    -fsanitize=address,undefined
)

target_link_options(oppenheimer-simulator PRIVATE
    -fsanitize=address,undefined
)
