cmake_minimum_required(VERSION 3.16)
project(cpp-abr-generator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
    libavformat
    libavcodec
    libavutil
    libswscale
)

add_executable(abr_generator
    src/main.cpp
    src/abr_encoder.cpp
)

target_include_directories(abr_generator PRIVATE include)

target_link_libraries(abr_generator PRIVATE PkgConfig::FFMPEG)

target_compile_options(abr_generator PRIVATE -Wall -Wextra)
