cmake_minimum_required(VERSION 3.29)
project(_idea)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BUILD_TYPE Debug)

add_executable(graphingTest ../graphingTest.cpp)
find_package(Matplot++ CONFIG REQUIRED)
target_link_libraries(graphingTest PRIVATE Matplot++::cimg  Matplot++::matplot)


add_executable(_idea
    codeStyles/codeStyleConfig.xml
    .gitignore
    cppAI.iml
    editor.xml
    misc.xml
    modules.xml
    vcs.xml
    workspace.xml
        ../array.h
#        ../cppTest.cpp
        ../layerClass.h
        ../graphingTest.cpp)
