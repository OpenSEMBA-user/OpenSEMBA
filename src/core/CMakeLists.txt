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

cmake_minimum_required(VERSION 3.0)

project(opensemba_core CXX)

include_directories(${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(argument/ argument/)
add_subdirectory(class/ class/)
add_subdirectory(exporter/ exporter/)
add_subdirectory(filesystem/ filesystem/)
add_subdirectory(geometry/ geometry/)
add_subdirectory(group/ group/)
add_subdirectory(math/ math/)
add_subdirectory(outputRequest/ outputRequest/)
add_subdirectory(parser/ parser/)
add_subdirectory(physicalModel/ physicalModel/)
add_subdirectory(solver/ solver/)
add_subdirectory(source/ source/)
add_subdirectory(util/ util/)

if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/Data.h AND EXISTS
          ${CMAKE_CURRENT_LIST_DIR}/Data.cpp)
    add_library(opensemba_core_data STATIC ${CMAKE_CURRENT_LIST_DIR}/Data.h
                                           ${CMAKE_CURRENT_LIST_DIR}/Data.cpp)
    target_link_libraries(opensemba_core_data opensemba_core_geometry
                                              opensemba_core_physicalmodel
                                              opensemba_core_source
                                              opensemba_core_outputrequest
                                              opensemba_core_filesystem)
endif()
