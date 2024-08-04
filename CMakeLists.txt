cmake_minimum_required(VERSION 3.16)

project(AA-Matrix DESCRIPTION "COMS4032A AA Assignment 1: Square Matrix Multiplication Algorithms" LANGUAGES C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED On)
set(CMAKE_C_EXTENSIONS Off)

# set(CMAKE_C_FLAGS "-O0")
# set(CMAKE_C_FLAGS "-O1")
# set(CMAKE_C_FLAGS "-O2")
# set(CMAKE_C_FLAGS "-O3")

add_executable(matrix-run
	src/main.c src/matrix.c src/algorithms.c
)
target_link_libraries(matrix-run m)
target_include_directories(matrix-run PUBLIC include)
target_link_directories(matrix-run PRIVATE src)

add_executable(matrix-test
	src/test.c src/matrix.c src/algorithms.c
)
target_link_libraries(matrix-test m)
target_include_directories(matrix-test PUBLIC include)
target_link_directories(matrix-test PRIVATE src)
