cmake_minimum_required(VERSION 2.8)

add_sources(. ${SRCS})

include_directories(${CMAKE_CURRENT_LIST_DIR})
add_library(common STATIC ${SRCS})