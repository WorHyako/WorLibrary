cmake_minimum_required(VERSION 3.24)

add_library(WorLib_Template INTERFACE)
add_library(Wor::Template ALIAS WorLib_Template)

target_include_directories(WorLib_Sql
        INTERFACE include)
