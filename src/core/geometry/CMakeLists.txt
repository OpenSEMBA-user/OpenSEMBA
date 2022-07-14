cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)

project(opensemba_core_geometry CXX)

add_sources(. SRCS)
add_library(opensemba_core_geometry STATIC ${SRCS})
target_link_libraries(opensemba_core_geometry opensemba_core_math)
