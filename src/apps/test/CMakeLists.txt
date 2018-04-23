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

if (GTEST_FOUND)
    include_directories(${GTEST_INCLUDE_DIRS}
                        ${CMAKE_CURRENT_LIST_DIR})
    add_sources(. SRCS)

    add_executable(opensemba_test ${SRCS})
    target_link_libraries(opensemba_test opensemba_core_argument
                                         opensemba_core_data
                                         opensemba_parser_json
                                         opensemba_parser_stl
                                         ${GTEST_BOTH_LIBRARIES})
endif()
