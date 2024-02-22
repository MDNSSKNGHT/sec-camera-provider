# CMakeLists for standalone use of injector

set(CMAKE_CXX_STANDARD 17)

set(MYINJECTOR_TARGET MyInjector)

add_library(${MYINJECTOR_TARGET} STATIC
        RemoteProcess.cpp
        Injector.cpp)
