cmake_minimum_required(VERSION 3.24)

set(WorLib_Dir ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
set(WorLib_Include_Dir ${CMAKE_CURRENT_SOURCE_DIR}/Include PARENT_SCOPE)

file(GLOB_RECURSE WorLib_Json ${CMAKE_CURRENT_SOURCE_DIR}/Src/Json/*.cpp)
file(GLOB_RECURSE WorLib_Vector ${WorLib_Include_Dir}/WorLibrary/Vector/*.hpp)
file(GLOB_RECURSE WorLib_Network ${CMAKE_CURRENT_SOURCE_DIR}/Src/Network/*.cpp)
file(GLOB_RECURSE WorLib_Sql ${CMAKE_CURRENT_SOURCE_DIR}/Src/Sql/*.cpp)

set(WorLib_Json ${WorLib_Json} PARENT_SCOPE)
set(WorLib_Sql ${WorLib_Sql} PARENT_SCOPE)
set(WorLib_Vector ${WorLib_Vector} PARENT_SCOPE)
set(WorLib_Network ${WorLib_Network} PARENT_SCOPE)
