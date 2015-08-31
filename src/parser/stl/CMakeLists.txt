cmake_minimum_required(VERSION 2.8)

add_sources(. ${SRCS})

add_library(parserSTL STATIC ${SRCS})
target_link_libraries(parserSTL common)