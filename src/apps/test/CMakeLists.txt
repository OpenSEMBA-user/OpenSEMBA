cmake_minimum_required(VERSION 2.8)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${CMAKE_CURRENT_LIST_DIR})
add_sources(. ${SRCS})

add_executable(testSemba ${SRCS})
target_link_libraries(testSemba opensemba parserGiD adapterFDTD semba gtest gtest_main)
set_target_properties(testSemba PROPERTIES OUTPUT_NAME test)