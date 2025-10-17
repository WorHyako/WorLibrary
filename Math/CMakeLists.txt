cmake_minimum_required(VERSION 3.24)

project(WorLibrary::Math
        LANGUAGES CXX
        VERSION 0.1)

add_library(WorLib_Math INTERFACE)
add_library(Wor::Math ALIAS WorLib_Math)

target_include_directories(WorLib_Math
        INTERFACE
        include)
