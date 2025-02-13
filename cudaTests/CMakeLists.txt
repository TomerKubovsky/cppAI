cmake_minimum_required(VERSION 3.16)
project(cudaTest LANGUAGES CUDA CXX)

set(CMAKE_CUDA_STANDARD 14)
set(CMAKE_CXX_STANDARD  20)
#add_executable(cudaMain kernel.cu)
add_executable(test cudaTestCpp.cu)

set_target_properties(
        untitiled
        PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON
)