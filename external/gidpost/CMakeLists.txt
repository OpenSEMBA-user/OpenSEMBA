cmake_minimum_required(VERSION 3.0)

project(gidpost C)
add_sources(. SRCS)
add_library(gidpost STATIC ${SRCS})

set(gidpost_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
