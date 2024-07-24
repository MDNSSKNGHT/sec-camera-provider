set(TARGET LibMadness)

find_package(shadowhook REQUIRED CONFIG)

add_library(${TARGET} SHARED OnLoad.cpp Proxies.cpp)
set_target_properties(${TARGET} PROPERTIES PREFIX "" OUTPUT_NAME "libmadness")
target_link_libraries(${TARGET} log shadowhook::shadowhook)
