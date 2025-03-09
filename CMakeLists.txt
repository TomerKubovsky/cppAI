cmake_minimum_required(VERSION 3.25.0)

#project(Main LANGUAGES CXX CUDA)
project(Main LANGUAGES CXX)

set(CMAKE_CUDA_STANDARD 20)

#set(CMAKE_CUDA_COMPILER "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.6/bin/nvcc.exe")  # Adjust path if needed

add_executable(MainProg openGlTests/utilsHeader.h main.cpp)
target_include_directories(MainProg PRIVATE "C:/Users/tomer/Downloads/glfw-3.4.bin.WIN64/glfw-3.4.bin.WIN64/include")
target_link_libraries(MainProg PRIVATE "C:/Users/tomer/Downloads/glfw-3.4.bin.WIN64/glfw-3.4.bin.WIN64/lib-vc2022/glfw3.lib")
target_link_libraries(MainProg PRIVATE "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/OpenGL32.Lib")

#add_executable(MainCuda
#        cudaTests/main.cu
#        mainAi/arrayV2.h
#        mainAi/layerClassV2.h
#        mainAi/cudaFuncs.h)

#add_executable(CudaTests cudaTests/cudaTestCpp.cu)

add_executable(MainAI mainAi/aiTests.cpp)

#set_target_properties(MainCuda PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
#set_target_properties(CudaTests PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
#
#target_include_directories(CudaTests PRIVATE "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.6/include")


add_executable(glTests openGlTests/openGlTests.cpp)
target_include_directories(glTests PRIVATE "C:/Users/tomer/Downloads/glfw-3.4.bin.WIN64/glfw-3.4.bin.WIN64/include")
target_link_libraries(glTests PRIVATE "C:/Users/tomer/Downloads/glfw-3.4.bin.WIN64/glfw-3.4.bin.WIN64/lib-vc2022/glfw3.lib")
target_link_libraries(glTests PRIVATE "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/OpenGL32.Lib")
target_link_options(glTests PRIVATE /NODEFAULTLIB:glfw3)

