cmake_minimum_required(VERSION 3.24)

project(WorLibrary
        VERSION 0.2
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

option(WORLIBRARY_JSON "" OFF)
option(WORLIBRARY_NETWORK "" OFF)
option(WORLIBRARY_SQL "" OFF)
option(WORLIBRARY_MATH "" OFF)
option(WORLIBRARY_CURRENCY "" OFF)
option(WORLIBRARY_TEMPLATEWRAPPERS "" OFF)
option(WORLIBRARY_DEFINEUTILS "" OFF)
option(WORLIBRARY_MIDI "" OFF)

#############
#   Boost   #
#############
set(Boost_NO_WARN_NEW_VERSIONS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_NO_BOOST_CMAKE ON)
find_package(Boost 1.85 REQUIRED GLOBAL COMPONENTS
        date_time
        thread)

if (WORLIBRARY_DEFINEUTILS)
    add_subdirectory(DefineUtils)
endif ()

if (WORLIBRARY_CURRENCY)
    add_subdirectory(Currency)
endif ()

if (WORLIBRARY_JSON)
    add_subdirectory(Json)
endif ()

if (WORLIBRARY_DATA_CONVERTER)
    add_subdirectory(DataConverter)
endif ()

if (WORLIBRARY_MATH)
    add_subdirectory(Math)
endif ()

if (WORLIBRARY_NETWORK)
    add_subdirectory(Network)
endif ()

if (WORLIBRARY_SQL)
    add_subdirectory(Sql)
endif ()

if (WORLIBRARY_TEMPLATE)
    add_subdirectory(TemplateWrappers)
endif ()

if (WORLIBRARY_MIDI)
    add_subdirectory(Midi)
endif ()
