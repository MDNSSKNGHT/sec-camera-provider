# CMakeLists for standalone use of vendor_samsung_hardware_camera_provider_service

set(CMAKE_CXX_STANDARD 17)

set(VENDOR_SAMSUNG_HARDWARE_CAMERA_PROVIDER_SERVICE_TARGET
        vendor_samsung_hardware_camera_provider_4_0_service)

add_executable(${VENDOR_SAMSUNG_HARDWARE_CAMERA_PROVIDER_SERVICE_TARGET}
        vendor_samsung_hardware_camera_provider_4_0_service.cpp)

target_link_libraries(${VENDOR_SAMSUNG_HARDWARE_CAMERA_PROVIDER_SERVICE_TARGET} log MyInjector)

target_link_options(${VENDOR_SAMSUNG_HARDWARE_CAMERA_PROVIDER_SERVICE_TARGET} PUBLIC -s)