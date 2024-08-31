cmake_minimum_required(VERSION 3.24)

##############
#   RtMidi   #
##############
find_package(PkgConfig REQUIRED)
pkg_check_modules(RtMidi IMPORTED_TARGET REQUIRED RtMidi)

add_library(WorLib_Midi)
add_library(Wor::Midi ALIAS WorLib_Midi)

file(GLOB_RECURSE Sources src/*.cpp)
target_sources(WorLib_Midi
        PRIVATE ${Sources})

target_include_directories(WorLib_Midi
        PUBLIC include
        PRIVATE
        include/Wor
        ${RtMidi_INCLUDEDIR})

target_link_libraries(WorLib_Midi
        PUBLIC
        PkgConfig::RtMidi
        Wor::Math)
