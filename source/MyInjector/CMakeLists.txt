set(TARGET MyInjector)

add_library(${TARGET} STATIC RemoteProcess.cpp Injector.cpp)
target_include_directories(${TARGET} PUBLIC .)
