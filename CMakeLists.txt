cmake_minimum_required(VERSION 3.15)
project(ga_allocator)

set(CMAKE_CXX_STANDARD 11)

add_executable(ga_allocator main.cpp include/Position.hpp src/StationNode.cpp include/StationNode.h src/Worker.cpp include/Worker.h src/Station.cpp include/Station.h src/GA.cpp include/GA.h include/io_utils.h src/io_utils.cpp include/constant_param.h)