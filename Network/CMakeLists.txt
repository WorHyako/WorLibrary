cmake_minimum_required(VERSION 3.24)

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
        ${Boost_LIBRARIES}
        spdlog::spdlog)
