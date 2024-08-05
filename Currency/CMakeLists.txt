cmake_minimum_required(VERSION 3.24)

add_library(WorLib_Currency)
add_library(Wor::Currency ALIAS WorLib_Currency)

file(GLOB_RECURSE Sources src/*.cpp)

target_sources(WorLib_Currency
        PRIVATE ${Sources})

target_include_directories(WorLib_Currency
        PUBLIC include
        PRIVATE include/Currency)
