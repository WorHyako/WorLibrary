cmake_minimum_required(VERSION 3.24)

add_library(WorLib_Sql)
add_library(Wor::Sql ALIAS WorLib_Sql)

file(GLOB_RECURSE Sources src/*.cpp)
target_sources(WorLib_Sql
        PRIVATE ${Sources})

target_include_directories(WorLib_Sql
        PUBLIC include
        PRIVATE include/Wor)

target_link_libraries(WorLib_Sql
        PUBLIC
        Wor::Converter
        Soci::core
        Soci::mysql)
