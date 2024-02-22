cmake_minimum_required(VERSION 3.22.1)

project(MyProject)

# MyInjector
add_subdirectory(MyInjector)

# libmadness
add_subdirectory(libmadness)

# vendor.samsung.hardware.camera.provider@4.0-service_64
add_subdirectory(vendor_samsung_hardware_camera_provider_4_0_service)
