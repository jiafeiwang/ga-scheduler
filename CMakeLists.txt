cmake_minimum_required(VERSION 3.15)
project(ga_scheduler)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

file(GLOB_RECURSE SRC_DIR_LIST "main.cpp" "src/*.cpp" "src/*.c")

file(GLOB_RECURSE INCLUDE_DIR_LIST "include/*.h" "include/*.hpp")

add_executable(ga_scheduler ${SRC_DIR_LIST} ${INCLUDE_DIR_LIST} src/Chromosome.cpp include/Chromosome.h src/gene.cpp include/gene.h src/Ga.cpp include/Ga.h)