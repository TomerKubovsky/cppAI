cmake_minimum_required(VERSION 3.29)
project(_idea)

set(CMAKE_CXX_STANDARD 23)
#set(CMAKE_BUILD_TYPE Debug)


add_definitions(-DOPENCV_STATIC)

set(OpenCV_DIR "C:/tools/opencv/build/x64/vc16/lib")

set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")  # Release build
set(BUILD_SHARED_LIBS  OFF)
#message(STATUS "include libd is ${OpenCV_LIBS}")
find_package(OpenCV REQUIRED)
add_executable(graphingTest ../graphingTest.cpp)
target_link_libraries(graphingTest ${OpenCV_LIBS})


target_link_directories(graphingTest PUBLIC ${OpenCV_LINK_DIRECTORIES})
#target_link_options(graphingTest PUBLIC pkg-config --cflags --libs opencv)
#target_compile_options(graphingTest PUBLIC /MTd)