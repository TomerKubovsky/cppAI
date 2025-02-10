cmake_minimum_required(VERSION 3.30)

project(Main LANGUAGES CXX CUDA)

set(CMAKE_CUDA_STANDARD 20)

add_executable(MainProg main.cpp)

add_executable(MainCuda
        cudaTests/main.cu
        mainAi/arrayV2.h
        mainAi/layerClassV2.h
        mainAi/cudaFuncs.h)

add_executable(MainAI mainAi/aiTests.cpp)

set_target_properties(MainCuda PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
