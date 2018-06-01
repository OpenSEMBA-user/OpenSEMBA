# OpenSEMBA
# Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
#                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
#                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
#                    Daniel Mateos Romero            (damarro@semba.guru)
#
# This file is part of OpenSEMBA.
#
# OpenSEMBA is free software: you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option)
# any later version.
#
# OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.8)

macro(add_subdirectories dir bin)
    set(curdir ${CMAKE_CURRENT_LIST_DIR}/${dir})
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)
    foreach(child ${children})
        if(IS_DIRECTORY ${curdir}/${child})
            if(EXISTS ${curdir}/${child}/CMakeLists.txt)
                add_subdirectory(${curdir}/${child} ${bin}/${dir}/${child})
            endif()
            if(EXISTS ${curdir}/${child}/src/CMakeLists.txt)
                add_subdirectory(${curdir}/${child}/src ${bin}/${dir}/${child})
            endif()
        endif()
    endforeach()
endmacro()

function(add_sources dir srcs)
    set(curdir ${CMAKE_CURRENT_LIST_DIR}/${dir})
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)
    foreach(child ${children})
        if(IS_DIRECTORY ${curdir}/${child})
            if (dir STREQUAL ".")
                add_sources(${child} ${srcs})
            else()
                add_sources(${dir}/${child} ${srcs})
            endif()
        endif()
    endforeach()
    file(GLOB HDRS_LOCAL RELATIVE ${CMAKE_CURRENT_LIST_DIR} ${curdir}/*.h)
    file(GLOB TMPL_LOCAL RELATIVE ${CMAKE_CURRENT_LIST_DIR} ${curdir}/*.hpp)
    file(GLOB SRCS_LOCAL RELATIVE ${CMAKE_CURRENT_LIST_DIR} ${curdir}/*.cpp)
    set(SRCS_LOCAL_ALL ${HDRS_LOCAL} ${TMPL_LOCAL} ${SRCS_LOCAL})
    set(${srcs} ${${srcs}} ${SRCS_LOCAL_ALL} PARENT_SCOPE)
    if (NOT dir STREQUAL ".")
        string(REPLACE "/" "\\\\" src_grp ${dir})
        source_group(${src_grp} FILES ${SRCS_LOCAL_ALL})
    endif()
endfunction()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory  (../external obj/external)


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

add_definitions(-DOPENSEMBA_VERSION="0.13")

include_directories(${CMAKE_CURRENT_LIST_DIR}/../external/
                    ${CMAKE_CURRENT_LIST_DIR}/../external/json/
                    ${CMAKE_CURRENT_LIST_DIR}
                    ${CMAKE_CURRENT_LIST_DIR}/core/)
add_subdirectory  (core     obj/src/core)
add_subdirectories(parser   obj/src)
add_subdirectory  (mesher   obj/src/mesher)
add_subdirectories(exporter obj/src)
add_subdirectories(adapter  obj/src)
add_subdirectory  (solver   obj/src/solver)
add_subdirectories(apps     obj/src)
