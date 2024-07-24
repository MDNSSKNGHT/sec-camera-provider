set(TARGET sec_camera_provider)

add_executable(${TARGET} sec_camera_provider.cpp)
target_link_libraries(${TARGET} MyInjector log)
