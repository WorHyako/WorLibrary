cmake_minimum_required(VERSION 3.24)

project(WorLibrary
        VERSION 0.1
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(WORLIBRARY_JSON "" OFF)
option(WORLIBRARY_NETWORK "" OFF)
option(WORLIBRARY_SQL "" OFF)
option(WORLIBRARY_MATH "" OFF)
option(WORLIBRARY_CURRENCY "" OFF)

set(Boost_USE_MULTITHREADED ON)
set(Boost_NO_BOOST_CMAKE ON)
set(Boost_NO_WARN_NEW_VERSIONS ON)
find_package(Boost 1.81.0 REQUIRED COMPONENTS date_time)

if (${WORLIBRARY_CURRENCY})
    file(GLOB_RECURSE WorLib_Currency_Src ${CMAKE_CURRENT_SOURCE_DIR}/Src/Currency/*.cpp)
    list(APPEND WorLib_Sources ${WorLib_Currency_Src})
endif ()

if (${WORLIBRARY_JSON})
    file(GLOB_RECURSE WorLib_Json_Src ${CMAKE_CURRENT_SOURCE_DIR}/Src/Json/*.cpp)
    list(APPEND WorLib_Sources ${WorLib_Json_Src})
endif ()

if(${WORLIBRARY_DATA_CONVERTER})
    file(GLOB_RECURSE WorLib_DataConverter_Src ${CMAKE_CURRENT_SOURCE_DIR}/Src/DataConverter/*.cpp)
    list(APPEND WorLib_Sources ${WorLib_DataConverter_Src})
endif ()

if (${WORLIBRARY_MATH})
endif ()

if (${WORLIBRARY_NETWORK})
    file(GLOB_RECURSE WorLib_Network_Sources ${CMAKE_CURRENT_SOURCE_DIR}/Src/Network/*.cpp)
    list(APPEND WorLib_Sources ${WorLib_Network_Sources})
endif ()

if (${WORLIBRARY_SQL})
    list(APPEND Wor_Compile_Defs "-DWORLIBRARY_SQL=1")
    set(Soci_Install_Root_Dir ${CMAKE_CURRENT_SOURCE_DIR}/../soci-install)
    option(SOCI_EMPTY "Build the sample backend called Empty" ON)
    find_package(Soci REQUIRED
            PATHS ${Soci_Install_Root_Dir}/lib/cmake/SOCI)

    find_library(mysqlcl mysqlclient REQUIRED)

    list(APPEND Soci_Libs
            soci_core
            soci_empty
            soci_mysql
            ${mysqlcl})
    list(APPEND Soci_Include_Dir
            /opt/homebrew/Cellar/mysql/8.1.0/include/mysql
            ${Soci_Install_Root_Dir}/include)
    list(APPEND Soci_Libs_Dir
            ${Soci_Install_Root_Dir}/lib/)

    file(GLOB_RECURSE WorLib_Sql_Sources
            ${CMAKE_CURRENT_SOURCE_DIR}/Src/Sql/*.cpp)
    list(APPEND WorLib_Sources ${WorLib_Sql_Sources})
endif ()

if (WorLib_Sources)
    add_library(${PROJECT_NAME} STATIC)
    target_include_directories(${PROJECT_NAME}
            PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/Include
            ${Soci_Include_Dir}
            ${Boost_INCLUDE_DIRS})
    target_sources(${PROJECT_NAME}
            PRIVATE
            ${WorLib_Sources})
    target_link_libraries(${PROJECT_NAME}
            PUBLIC
            ${Soci_Libs}
            ${Boost_LIBRARIES})
    target_link_directories(${PROJECT_NAME}
            PUBLIC
            ${Soci_Libs_Dir})
    target_compile_definitions(${PROJECT_NAME}
            PUBLIC
            ${Wor_Compile_Defs})
endif ()

set(WorLib_Include_Dir ${CMAKE_CURRENT_SOURCE_DIR}/Include PARENT_SCOPE)
set(Wor_Compile_Defs ${Wor_Compile_Defs} PARENT_SCOPE)
