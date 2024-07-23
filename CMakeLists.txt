cmake_minimum_required(VERSION 3.22.1)
project(sec_camera_provider_wrapper)

set(CMAKE_CXX_STANDARD 17)

add_executable(sec_camera_provider_wrapper main.cpp)
target_link_libraries(sec_camera_provider_wrapper log)
