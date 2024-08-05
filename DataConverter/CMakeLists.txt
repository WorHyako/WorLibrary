cmake_minimum_required(VERSION 3.24)

add_library(WorLib_Converter)
add_library(Wor::Converter ALIAS WorLib_Converter)

file(GLOB_RECURSE Sources src/*.cpp)

target_sources(WorLib_Converter
        PRIVATE ${Sources})

target_include_directories(WorLib_Converter
        PUBLIC include
        PRIVATE include/DataConverter)

target_link_libraries(WorLib_Converter
        PUBLIC Soci::core)
