cmake_minimum_required(VERSION 3.6)

project(ShinyAdvancedSample LANGUAGES C CXX)

set(ADVANCED_SOURCES
    Main.cpp
    Advanced.cpp
    Shared.h)

add_executable(ShinyAdvancedSample ${ADVANCED_SOURCES})
target_link_libraries(ShinyAdvancedSample PRIVATE Shiny)
set_target_properties(ShinyAdvancedSample PROPERTIES FOLDER ${SHINY_BASE_FOLDER}/Samples/)