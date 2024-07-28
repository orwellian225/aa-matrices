cmake_minimum_required(VERSION 3.16)

project(AA-Matrix DESCRIPTION "COMS4032A AA Assignment 1: Square Matrix Multiplication Algorithms" LANGUAGES C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED On)
set(CMAKE_C_EXTENSIONS Off)

add_executable(matrix-testing
	src/main.c src/matrix.c src/algorithms.c
)
target_include_directories(matrix-testing PUBLIC include)
target_link_directories(matrix-testing PRIVATE src)
