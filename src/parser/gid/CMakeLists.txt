cmake_minimum_required(VERSION 2.8)

add_sources(. ${SRCS})

add_library(parserGiD STATIC ${SRCS})
target_link_libraries(parserGiD common)