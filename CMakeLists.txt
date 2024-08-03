cmake_minimum_required(VERSION 3.24)

project(WorLibrary
        VERSION 0.2
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(WORLIBRARY_JSON "" OFF)
option(WORLIBRARY_NETWORK "" OFF)
option(WORLIBRARY_SQL "" OFF)
option(WORLIBRARY_MATH "" OFF)
option(WORLIBRARY_CURRENCY "" OFF)
option(WORLIBRARY_TEMPLATE "" OFF)

set(Boost_NO_WARN_NEW_VERSIONS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_NO_BOOST_CMAKE ON)
find_package(Boost 1.79...1.85 REQUIRED GLOBAL COMPONENTS
        date_time
        thread)

if (WORLIBRARY_CURRENCY)
    add_library(WorLib_Currency)
    add_library(Wor::Currency ALIAS WorLib_Currency)

    file(GLOB_RECURSE Sources Src/Currency/*.cpp)

    target_sources(WorLib_Currency
            PRIVATE ${Sources})

    target_include_directories(WorLib_Currency
            PUBLIC Include)

    unset(Sources)
endif ()

if (WORLIBRARY_JSON)
    add_library(WorLib_Json)
    add_library(Wor::Json ALIAS WorLib_Json)

    file(GLOB_RECURSE Sources Src/Json/*.cpp)
    target_sources(WorLib_Json
            PRIVATE ${Sources})

    target_include_directories(WorLib_Json
            PUBLIC Include)

    target_link_libraries(WorLib_Json
            PUBLIC nlohmann_json::nlohmann_json)

    unset(Sources)
endif ()

if (WORLIBRARY_DATA_CONVERTER)
    add_library(WorLib_Converter)
    add_library(Wor::Converter ALIAS WorLib_Converter)

    file(GLOB_RECURSE Sources Src/DataConverter/*.cpp)

    target_sources(WorLib_Converter
            PRIVATE ${Sources})

    target_include_directories(WorLib_Converter
            PUBLIC Include)

    target_link_libraries(WorLib_Converter
            PUBLIC Soci::core)

    unset(Sources)
endif ()

if (WORLIBRARY_MATH)
    add_library(WorLib_Math INTERFACE)
    add_library(Wor::Math ALIAS WorLib_Math)

    target_include_directories(WorLib_Math
            INTERFACE Include)
endif ()

if (WORLIBRARY_NETWORK)
    add_library(WorLib_Network)
    add_library(Wor::Network ALIAS WorLib_Network)

    file(GLOB_RECURSE Sources Src/Network/*.cpp)

    target_sources(WorLib_Network
            PRIVATE ${Sources})

    target_include_directories(WorLib_Network
            PUBLIC
            Include
            ${Boost_INCLUDE_DIR})

    target_link_libraries(WorLib_Network
            PUBLIC ${Boost_LIBRARIES})

    unset(Sources)
endif ()

if (WORLIBRARY_SQL)
    add_library(WorLib_Sql)
    add_library(Wor::Sql ALIAS WorLib_Sql)

    file(GLOB_RECURSE Sources Src/Sql/*.cpp)
    target_sources(WorLib_Sql
            PRIVATE ${Sources})

    target_include_directories(WorLib_Sql
            PUBLIC Include)

    target_link_libraries(WorLib_Sql
            PUBLIC Soci::core
            Soci::mysql)

    unset(Sources)
endif ()

if (WORLIBRARY_TEMPLATE)
    add_library(WorLib_Template INTERFACE)
    add_library(Wor::Template ALIAS WorLib_Template)

    target_include_directories(WorLib_Sql
            INTERFACE Include)

    unset(Sources)
endif ()