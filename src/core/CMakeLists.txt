

cmake_minimum_required(VERSION 3.0)

project(opensemba_core CXX)

include_directories(${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(argument/ argument/)
add_subdirectory(class/ class/)
add_subdirectory(filesystem/ filesystem/)
add_subdirectory(geometry/ geometry/)
add_subdirectory(group/ group/)
add_subdirectory(math/ math/)
add_subdirectory(outputRequest/ outputRequest/)
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
