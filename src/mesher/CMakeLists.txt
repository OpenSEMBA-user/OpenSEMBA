

cmake_minimum_required(VERSION 3.0)

project(opensemba_mesher CXX)

add_sources(. SRCS)

add_library(opensemba_mesher STATIC ${SRCS})
