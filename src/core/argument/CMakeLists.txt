

cmake_minimum_required(VERSION 3.0)

project(opensemba_core_argument CXX)

include_directories(${CMAKE_CURRENT_LIST_DIR})

add_sources(. SRCS)

add_library(opensemba_core_argument STATIC ${SRCS})