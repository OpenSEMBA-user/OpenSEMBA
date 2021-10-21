

cmake_minimum_required(VERSION 3.0)

project(opensemba_mesher CXX)

add_sources(. SRCS)

add_library(opensemba_mesher STATIC ${SRCS})

find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(opensemba_mesher nlohmann_json)
