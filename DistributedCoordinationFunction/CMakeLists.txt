cmake_minimum_required(VERSION 3.3.2)
project(DCF)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

add_library(DCF SHARED src/libDCF.c)

add_executable(A1Sim src/A1sim.c)
target_link_libraries(A1Sim DCF m)

add_executable(A2Sim src/A2sim.c)
target_link_libraries(A2Sim DCF m)

add_executable(B1Sim src/B1sim.c)
target_link_libraries(B1Sim DCF m)

add_executable(B2Sim src/B2sim.c)
target_link_libraries(B2Sim DCF m)