cmake_minimum_required(VERSION 3.16)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Notebook2021Cpp1)

add_compile_options(-Wall -Wextra -pedantic)
add_compile_options(-Wno-unused-function -Wno-unused-parameter -Wno-padded -Wno-sign-conversion -Wno-sign-compare)

set(Notebook2021Cpp1_Sources
    main.cpp
)

add_executable(Notebook2021Cpp1 ${Notebook2021Cpp1_Sources})
