cmake_minimum_required(VERSION 2.8)

add_sources(. ${SRCS})

add_library(exporterGiD STATIC ${SRCS})
target_link_libraries(exporterGiD opensemba gidpost)