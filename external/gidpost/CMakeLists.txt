cmake_minimum_required(VERSION 2.8)

project(gidpost C)

file(GLOB_RECURSE HDRS RELATIVE ${CMAKE_CURRENT_LIST_DIR} ${curdir}/*.h)
file(GLOB_RECURSE SRCS RELATIVE ${CMAKE_CURRENT_LIST_DIR} ${curdir}/*.c)

add_library(gidpost STATIC ${HDRS} ${SRCS})