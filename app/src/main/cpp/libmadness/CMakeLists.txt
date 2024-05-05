# CMakeLists for standalone use of libmadness

set(CMAKE_CXX_STANDARD 17)

set(LIBMADNESS_TARGET madness)

find_package(shadowhook REQUIRED CONFIG)

add_library(${LIBMADNESS_TARGET} SHARED OnLoad.cpp Proxies.cpp)

target_link_libraries(${LIBMADNESS_TARGET} log shadowhook::shadowhook)