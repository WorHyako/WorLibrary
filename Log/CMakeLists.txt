cmake_minimum_required(VERSION 3.24)

project(WorLibrary::Log
        VERSION 0.1
        LANGUAGES CXX)

option(WOR_DEV_LOG "" OFF)

# ---------- #
#   spdlog   #
# ---------- #
find_package(spdlog GLOBAL REQUIRED)
target_compile_definitions(spdlog::spdlog
        INTERFACE SPDLOG_ACTIVE_LEVEL=0)

add_library(WorLib_Log INTERFACE)
add_library(Wor::Log ALIAS WorLib_Log)

if (WOR_DEV_LOG)
    target_compile_definitions(WorLib_Log
            INTERFACE WOR_DEV_LOG)
endif ()

target_include_directories(WorLib_Log
        INTERFACE include)

target_link_libraries(WorLib_Log
        INTERFACE spdlog::spdlog)
