

cmake_minimum_required(VERSION 3.0)

project(opensemba_core_physicalmodel CXX)

add_sources(. SRCS)
add_library(opensemba_core_physicalmodel STATIC ${SRCS})
target_link_libraries(opensemba_core_physicalmodel opensemba_core_math
                                                   opensemba_core_filesystem)
