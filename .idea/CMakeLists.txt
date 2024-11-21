cmake_minimum_required(VERSION 3.29)
project(_idea)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Debug)

#CMAKE_PREFIX_PATH(
#        C:/Users/tomer/AppData/Local/Programs/Python/Python310
#        ${CMAKE_BINARY_DIR}/Python310
#)
add_subdirectory(
#        C:/Users/tomer/matplotplusplus
#        ${CMAKE_BINARY_DIR}/matplotplusplus
        C:/opencv/sources
        ${CMAKE_BINARY_DIR}/opencv
)



find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(graphingTest ../graphingTest.cpp)
target_link_libraries(graphingTest ${OpenCV_LIBS})
#target_link_libraries(graphingTest PUBLIC matplot)



#add_executable(_idea
#    codeStyles/codeStyleConfig.xml
#    .gitignore
#    cppAI.iml
#    editor.xml
#    misc.xml
#    modules.xml
#    vcs.xml
#    workspace.xml
#        ../array.h
##        ../cppTest.cpp
#        ../layerClass.h
#        ../graphingTest.cpp)
