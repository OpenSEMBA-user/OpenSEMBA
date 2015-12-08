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

add_subdirectories(. .)