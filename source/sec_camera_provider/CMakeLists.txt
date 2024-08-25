set(TARGET sec_camera_provider)

if(${ANDROID_ABI} STREQUAL "arm64-v8a")
    set(ARCH "arm64")
elseif(${ANDROID_ABI} STREQUAL "armeabi-v7a")
    set(ARCH "arm")
endif()

find_package(mettle_libreflect REQUIRED CONFIG)

add_executable(${TARGET} sec_camera_provider.cpp)
target_link_libraries(${TARGET} MyInjector log mettle_libreflect)
target_include_directories(${TARGET} PUBLIC ./${ARCH})