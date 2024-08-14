set(TARGET sec_camera_provider)

find_package(mettle_libreflect REQUIRED CONFIG)

add_executable(${TARGET} sec_camera_provider.cpp)
target_link_libraries(${TARGET} MyInjector log mettle_libreflect)
target_include_directories(${TARGET} PUBLIC ./${ANDROID_ABI})