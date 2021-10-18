

cmake_minimum_required(VERSION 3.0)

project(opensemba_solver CXX)

add_sources(. SRCS)
add_library(opensemba_solver STATIC ${SRCS})
target_link_libraries(opensemba_solver opensemba_core_argument
                                       opensemba_core_math)
