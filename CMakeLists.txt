cmake_minimum_required(VERSION 3.18)

project("lab4" LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
        matrix_multiplier.cu
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        CUDA_ARCHITECTURES 86
        CUDA_SEPARABLE_COMPILATION ON
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/W4 /utf-8>)
    target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:-Xcompiler=/W4,/utf-8>)
endif()