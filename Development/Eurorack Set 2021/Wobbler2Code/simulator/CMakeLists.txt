cmake_minimum_required(VERSION 3.7)

project(Wobbler2Simulator)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(Wobbler2Simulator wobbler2_simulator_main.cpp)
target_link_libraries(Wobbler2Simulator ${SDL2_LIBRARIES})

