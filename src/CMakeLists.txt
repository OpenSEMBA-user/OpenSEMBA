cmake_minimum_required(VERSION 2.8)

project(opensemba CXX)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("-std=c++11" compiler_supports_cxx11)
    check_cxx_compiler_flag("-std=c++0x" compiler_supports_cxx0X)
    if(compiler_supports_cxx11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    elseif(compiler_supports_cxx0X)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    else()
        message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. "
                            "Please use a different C++ compiler.")
    endif()
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(curdir ${CMAKE_CURRENT_LIST_DIR}/${dir})
file(GLOB_RECURSE HDRS_LOCAL RELATIVE ${CMAKE_CURRENT_LIST_DIR} ${curdir}/*.h)
file(GLOB_RECURSE TMPL_LOCAL RELATIVE ${CMAKE_CURRENT_LIST_DIR} ${curdir}/*.hpp)
file(GLOB_RECURSE SRCS_LOCAL RELATIVE ${CMAKE_CURRENT_LIST_DIR} ${curdir}/*.cpp)

list(APPEND SRCS ${HDRS_LOCAL} ${TMPL_LOCAL} ${SRCS_LOCAL})

add_definitions(-DAPP_VERSION="0.10")

include_directories(${CMAKE_CURRENT_LIST_DIR})

add_library(opensemba STATIC ${SRCS})
