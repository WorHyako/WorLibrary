cmake_minimum_required(VERSION 3.24)

project(WorLibrary::Json
        VERSION 0.1
        LANGUAGES CXX)

add_library(WorLib_Json)
add_library(Wor::Json ALIAS WorLib_Json)

file(GLOB_RECURSE Sources src/*.cpp)
target_sources(WorLib_Json
        PRIVATE
        ${Sources})

target_include_directories(WorLib_Json
        PUBLIC
        include
        PRIVATE
        include/Wor)

target_link_libraries(WorLib_Json
        PUBLIC
        nlohmann_json::nlohmann_json
        spdlog::spdlog)
