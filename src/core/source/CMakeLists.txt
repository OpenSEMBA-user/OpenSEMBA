

cmake_minimum_required(VERSION 3.0)

project(opensemba_core_source CXX)

add_sources(. SRCS)
add_library(opensemba_core_source STATIC ${SRCS})
target_link_libraries(opensemba_core_source opensemba_core_geometry
                                            opensemba_core_filesystem)
