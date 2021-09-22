

cmake_minimum_required(VERSION 3.0)

project(opensemba_core_group CXX)

include_directories(${CMAKE_CURRENT_LIST_DIR})

add_sources(. SRCS)
add_library(opensemba_core_group STATIC ${SRCS})
set_target_properties(opensemba_core_group PROPERTIES LINKER_LANGUAGE CXX)
