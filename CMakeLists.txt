cmake_minimum_required(VERSION 3.24)

set(worLib_Dir ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
set(worLib_Include_Dir ${CMAKE_CURRENT_SOURCE_DIR}/Include PARENT_SCOPE)

file(GLOB_RECURSE worLib_Json ${CMAKE_CURRENT_SOURCE_DIR}/Src/json/*.cpp)
file(GLOB_RECURSE worLib_Vector ${CMAKE_CURRENT_SOURCE_DIR}/Include/vector/*.hpp)

set(worLib_Json ${worLib_Json} PARENT_SCOPE)
set(worLib_Vector ${worLib_Vector} PARENT_SCOPE)
