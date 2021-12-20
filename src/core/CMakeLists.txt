cmake_minimum_required(VERSION 3.0)

project(opensemba_core CXX)

include_directories(${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(class/ class/)
add_subdirectory(filesystem/ filesystem/)
add_subdirectory(geometry/ geometry/)
add_subdirectory(group/ group/)
add_subdirectory(math/ math/)
add_subdirectory(outputRequest/ outputRequest/)
add_subdirectory(physicalModel/ physicalModel/)
add_subdirectory(source/ source/)

if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/Data.h AND EXISTS
          ${CMAKE_CURRENT_LIST_DIR}/Data.cpp)

    add_library(opensemba_core STATIC ${CMAKE_CURRENT_LIST_DIR}/Data.h
                                           ${CMAKE_CURRENT_LIST_DIR}/Data.cpp)

    find_package(nlohmann_json CONFIG REQUIRED)

    target_link_libraries(opensemba_core opensemba_core_geometry
                                              opensemba_core_physicalmodel
                                              opensemba_core_source
                                              opensemba_core_outputrequest
                                              opensemba_core_filesystem
                                              nlohmann_json)
endif()
