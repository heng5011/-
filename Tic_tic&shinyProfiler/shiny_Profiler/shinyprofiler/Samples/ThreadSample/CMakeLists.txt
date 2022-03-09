cmake_minimum_required(VERSION 3.6)

project(ShinyThreadSample LANGUAGES C CXX)

set(SOURCES ThreadSample.cpp)

add_executable(ShinyThreadSample ${SOURCES})
target_link_libraries(ShinyThreadSample PRIVATE Shiny)

set_target_properties(
    ShinyThreadSample 
    PROPERTIES 
    FOLDER ${SHINY_BASE_FOLDER}/Samples/)

if (UNIX OR ANDROID)
  # UNIX builds require executable to set the pthread flag
  target_link_libraries(ShinyThreadSample PRIVATE pthread)
endif()
