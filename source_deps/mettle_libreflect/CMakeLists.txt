cmake_minimum_required(VERSION 3.22.1)
project(mettle_libreflect)

set(TARGET libreflect)
set(ARCH aarch64)

file(GLOB SRC src/*.c)
add_library(${TARGET} SHARED ${SRC})
set_target_properties(${TARGET} PROPERTIES PREFIX "")
target_compile_options(${TARGET} PUBLIC -Wall -Werror -pedantic -std=gnu99)
target_include_directories(${TARGET} PUBLIC include arch/linux/${ARCH})
target_link_libraries(${TARGET} log)
