cmake_minimum_required(VERSION 4.1)
project(mylang)

include_directories(${CMAKE_SOURCE_DIR}/src)


add_executable(mylang
    src/main.c
    src/tok.c
    src/asm/asm.c
    src/asm/constexpr.c

    src/asm/platform/x86_64/match.c
    src/asm/platform/x86_64/mnems.c
    src/asm/platform/x86_64/parse.c
    src/asm/platform/x86_64/encode.c

)