cmake_minimum_required(VERSION 3.6)

project(ShinyMainLoopSample LANGUAGES CXX)

set(MAINLOOP_SOURCES MainLoop.cpp)

add_executable(ShinyMainLoopSample ${MAINLOOP_SOURCES})
target_link_libraries(ShinyMainLoopSample PRIVATE Shiny)
set_target_properties(ShinyMainLoopSample PROPERTIES FOLDER ${SHINY_BASE_FOLDER}/Samples/)