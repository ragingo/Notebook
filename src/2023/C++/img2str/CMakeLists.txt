cmake_minimum_required(VERSION 3.18)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Notebook2023Cpp_img2str)

add_compile_options(-Wall -Wextra -pedantic)
add_compile_options(-Wno-unused-function -Wno-unused-parameter -Wno-padded -Wno-sign-conversion -Wno-sign-compare)

set(Notebook2023Cpp_img2str_Sources
    main.cpp
)

add_executable(app ${Notebook2023Cpp_img2str_Sources})
