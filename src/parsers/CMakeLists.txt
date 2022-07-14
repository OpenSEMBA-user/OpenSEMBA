cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)

project(opensemba_parsers CXX)
add_sources(. SRCS)

add_library(opensemba_parsers STATIC ${SRCS})
target_link_libraries(opensemba_parsers opensemba_core)
