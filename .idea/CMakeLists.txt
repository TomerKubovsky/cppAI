cmake_minimum_required(VERSION 3.29)
project(_idea)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Debug)


set(OpenCV_DIR "C:/tools/opencv/build/x64/vc16/lib")

message(STATUS "OpenCV_DIR is set to ${OpenCV_DIR}")

find_package(OpenCV REQUIRED)

message(STATUS "Found OpenCV version ${OpenCV_VERSION}")
message(STATUS "OpenCV include dirs: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV link libs: ${OpenCV_LIBS}")

include_directories(
        ${OpenCV_INCLUDE_DIRS}
) # Not needed for CMake >= 2.8.11
add_executable(graphingTest ../graphingTest.cpp)
target_include_directories(graphingTest PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(graphingTest ${OpenCV_LIBS})
target_link_directories(graphingTest PUBLIC ${OpenCV_LINK_DIRECTORIES})
#target_link_options(graphingTest PUBLIC pkg-config --cflags --libs opencv)
