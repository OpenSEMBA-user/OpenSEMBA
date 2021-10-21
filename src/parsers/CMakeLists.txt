

cmake_minimum_required(VERSION 3.0)

project(opensemba_parsers CXX)
add_sources(. SRCS)

add_library(opensemba_parsers STATIC ${SRCS})
target_link_libraries(opensemba_parsers opensemba_core_data opensemba_core_util)
