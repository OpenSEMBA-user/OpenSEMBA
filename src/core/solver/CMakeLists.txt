

cmake_minimum_required(VERSION 3.0)

project(opensemba_core_solver CXX)

add_sources(. SRCS)
add_library(opensemba_core_solver STATIC ${SRCS})
target_link_libraries(opensemba_core_solver opensemba_core_data)
