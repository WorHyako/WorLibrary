cmake_minimum_required(VERSION 3.24)

add_library(WorLib_Wrappers INTERFACE)
add_library(Wor::Wrappers ALIAS WorLib_Wrappers)

target_include_directories(WorLib_Wrappers
        INTERFACE include)
