cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)

project(opensemba_exporters CXX)

add_sources(. SRCS)


include_directories(${gidpost_INCLUDES})
add_library(opensemba_exporters STATIC ${SRCS})
target_link_libraries(opensemba_exporters opensemba_core gidpost)
