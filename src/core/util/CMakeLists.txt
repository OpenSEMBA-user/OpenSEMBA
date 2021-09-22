

cmake_minimum_required(VERSION 3.0)

project(opensemba_core_util CXX)

add_sources(. SRCS)
add_library(opensemba_core_util STATIC ${SRCS})
target_link_libraries(opensemba_core_util opensemba_core_data)
set_target_properties(opensemba_core_util PROPERTIES LINKER_LANGUAGE CXX)
