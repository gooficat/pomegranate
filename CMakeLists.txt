cmake_minimum_required(VERSION 4.1)
project(mylang)

include_directories(${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE srcs CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*)

add_executable(mylang
    ${srcs}
)