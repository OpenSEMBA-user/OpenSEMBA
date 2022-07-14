cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)

project(opensemba_core_math CXX)


include_directories(${CMAKE_CURRENT_LIST_DIR})

add_sources(. SRCS)
add_library(opensemba_core_math STATIC ${SRCS})
