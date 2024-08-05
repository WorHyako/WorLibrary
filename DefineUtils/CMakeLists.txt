cmake_minimum_required(VERSION 3.24)

add_library(WorLib_DefineUtils INTERFACE)
add_library(Wor::DefineUtils ALIAS WorLib_DefineUtils)

target_include_directories(WorLib_DefineUtils
        INTERFACE include)
