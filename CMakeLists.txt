cmake_minimum_required(VERSION 3.22.1)
project(sec_camera_provider_wrapper)

set(shadowhook_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/shadowhook)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_subdirectory(source/MyInjector)

add_subdirectory(source/LibMadness)

add_subdirectory(source/sec_camera_provider)