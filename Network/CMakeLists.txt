cmake_minimum_required(VERSION 3.24)

project(WorLibrary::Network
        VERSION 1.0
        LANGUAGES CXX)

add_library(WorLib_Network)
add_library(Wor::Network ALIAS WorLib_Network)

file(GLOB_RECURSE Sources src/*.cpp)

target_sources(WorLib_Network
        PRIVATE ${Sources})

target_include_directories(WorLib_Network
        PUBLIC
        include
        ${Boost_INCLUDE_DIR}
        PRIVATE include/Wor)

target_link_libraries(WorLib_Network
        PUBLIC
        Boost::date_time
        Boost::thread
        Wor::Log)
