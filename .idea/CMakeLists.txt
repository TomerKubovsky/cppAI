cmake_minimum_required(VERSION 3.29)
project(_idea)

set(CMAKE_CXX_STANDARD 14)

add_executable(_idea
    codeStyles/codeStyleConfig.xml
    .gitignore
    cppAI.iml
    editor.xml
    misc.xml
    modules.xml
    vcs.xml
    workspace.xml
        ../findPixelValueTest.cpp
        ../array.hpp
        ../cppTest.cpp
        ../layerClass.h)
