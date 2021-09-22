

cmake_minimum_required(VERSION 3.0)

project(opensemba_core_outputrequest CXX)

add_sources(. SRCS)
add_library(opensemba_core_outputrequest STATIC ${SRCS})
target_link_libraries(opensemba_core_outputrequest opensemba_core_geometry)
