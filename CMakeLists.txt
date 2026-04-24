cmake_minimum_required(VERSION 3.20)
project("lab5" CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(MPI REQUIRED)

add_executable(${PROJECT_NAME}
      matrix_multiplier.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE MPI::MPI_CXX)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wall" "-O3" "-march=native")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wall" "-O3" "-xHost")
endif()