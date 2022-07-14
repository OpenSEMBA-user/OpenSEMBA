cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)

message(STATUS "Configuring build system for opensemba tests.")

find_package(GTest CONFIG REQUIRED)

add_subdirectories(. .)
add_sources(. SRCS)

find_package(GTest CONFIG REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(opensemba_tests ${SRCS})

target_link_libraries(opensemba_tests
                        GTest::gtest 
		                GTest::gtest_main
                        opensemba_core
                        opensemba_parsers)

add_test(opensemba  ${CMAKE_BINARY_DIR}/bin/opensemba_tests)


