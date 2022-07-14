cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)

project(opensemba_core_model CXX)

add_sources(. SRCS)
add_library(opensemba_core_model STATIC ${SRCS})
