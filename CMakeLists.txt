cmake_minimum_required(VERSION 3.30)

project(Main LANGUAGES CXX CUDA)

set(CMAKE_CUDA_STANDARD 20)

set(CMAKE_CUDA_COMPILER "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.6/bin/nvcc.exe")  # Adjust path if needed

add_executable(MainProg main.cpp)

add_executable(MainCuda
        cudaTests/main.cu
        mainAi/arrayV2.h
        mainAi/layerClassV2.h
        mainAi/cudaFuncs.h)

add_executable(CudaTests cudaTests/cudaTestCpp.cu)

add_executable(MainAI mainAi/aiTests.cpp)

set_target_properties(MainCuda PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
set_target_properties(CudaTests PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

target_include_directories(CudaTests PRIVATE "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.6/include")
