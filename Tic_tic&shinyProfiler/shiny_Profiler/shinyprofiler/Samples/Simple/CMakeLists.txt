cmake_minimum_required(VERSION 3.6)

project(ShinySimpleSample LANGUAGES C CXX)

set(SIMPLE_SOURCES Simple.cpp)

add_executable(ShinySimpleSample ${SIMPLE_SOURCES})
target_link_libraries(ShinySimpleSample PRIVATE Shiny)
set_target_properties(ShinySimpleSample PROPERTIES FOLDER ${SHINY_BASE_FOLDER}/Samples/)