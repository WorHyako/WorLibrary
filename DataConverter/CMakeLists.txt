cmake_minimum_required(VERSION 3.24)

# -------- #
#   soci   #
# -------- #
set(SOCI_EMPTY OFF)
set(SOCI_TESTS OFF)
set(SOCI_VISIBILITY OFF)
set(SOCI_FIREBIRD OFF)
set(SOCI_SQLITE3 OFF)
set(SOCI_ORACLE OFF)
set(SOCI_ODBC OFF)
set(SOCI_POSTGRESQL OFF)
set(SOCI_DB2 OFF)
find_package(SOCI CONFIG REQUIRED GLOBAL)

add_library(WorLib_Converter)
add_library(Wor::Converter ALIAS WorLib_Converter)

file(GLOB_RECURSE Sources src/*.cpp)

target_sources(WorLib_Converter
        PRIVATE
        ${Sources})

target_include_directories(WorLib_Converter
        PUBLIC
        include
        PRIVATE
        include/DataConverter)

target_link_libraries(WorLib_Converter
        PUBLIC
        SOCI::soci_core_static)
