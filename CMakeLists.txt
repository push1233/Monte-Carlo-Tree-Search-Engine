cmake_minimum_required(VERSION 3.10)
project(LockFreeMCTS VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pthread -march=native")

# Tell CMake the header file (mcts.h) is in the current directory
include_directories(.)

# Tell CMake the C++ files are in the current directory, not a 'src' folder
add_executable(mcts_engine main.cpp mcts.cpp)