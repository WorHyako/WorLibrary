cmake_minimum_required(VERSION 3.24)

project(WorLibrary::DefineUtils
        VERSION 0.1
        LANGUAGES CXX)

add_library(WorLib_DefineUtils INTERFACE)
add_library(Wor::DefineUtils ALIAS WorLib_DefineUtils)

target_include_directories(WorLib_DefineUtils
        INTERFACE
        include)
