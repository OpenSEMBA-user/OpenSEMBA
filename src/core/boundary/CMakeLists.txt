

cmake_minimum_required(VERSION 3.0)

project(opensemba_core_boundary CXX)

add_sources(. SRCS)
add_library(opensemba_core_boundary STATIC ${SRCS})
