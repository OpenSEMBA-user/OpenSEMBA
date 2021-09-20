cmake_minimum_required(VERSION 3.0)

project(gidpost C)
add_sources(. SRCS)
add_library(gidpost STATIC ${SRCS})


