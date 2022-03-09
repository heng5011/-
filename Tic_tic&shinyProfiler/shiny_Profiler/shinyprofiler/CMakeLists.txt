cmake_minimum_required(VERSION 3.6)

project(Shiny LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14) #baoer
#判断编译器类型,如果是gcc编译器,则在编译选项中加入c++11支持
IF(CMAKE_COMPILER_IS_GNUCXX)
    ADD_COMPILE_OPTIONS(-std=c++11)
    MESSAGE(STATUS "optional:-std=c++11")   
ENDIF(CMAKE_COMPILER_IS_GNUCXX)



# SHINY_HOSTED_BUILD should be set to TRUE when Shiny is included
# in another project's CMake build

if (SHINY_HOSTED_BUILD)
    set(_SHINY_DEFAULT_BUILD_SAMPLES OFF)
else()
    set(_SHINY_DEFAULT_BUILD_SAMPLES ON)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()

set(SHINY_BUILD_SAMPLES ${_SHINY_DEFAULT_BUILD_SAMPLES} CACHE BOOL "Build Shiny samples")
set(SHINY_VERBOSE_ANDROID_LOGGING FALSE CACHE BOOL "Enable verbose Android logging")
unset(_SHINY_DEFAULT_BUILD_SAMPLES)

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")

# set(CMAKE_CXX_FLAGS   "-fdump-rtl-expand")  
# set(CMAKE_C_FLAGS   "-fdump-rtl-expand")  

set(SHINY_HEADERS
    # include/tictoc.h
    # include/Timer.h

    include/Shiny.h
    include/ShinyConfig.h
    include/ShinyData.h
    include/ShinyMacros.h
    include/ShinyManager.h
    include/ShinyNode.h
    include/ShinyNodePool.h
    include/ShinyNodeState.h
    include/ShinyOutput.h
    include/ShinyPrereqs.h
    include/ShinyTools.h
    include/ShinyVersion.h
    include/ShinyZone.h)

set(SHINY_SOURCES
    # src/tictoc.cpp
    # src/Timer.cpp

    src/ShinyManager.cpp
    src/ShinyNode.cpp #baoer
    src/ShinyNodePool.c
    src/ShinyNodeState.c
    src/ShinyOutput.cpp
    src/ShinyTools.cpp
    src/ShinyZone.c)

if (NOT WIN32)
  add_definitions(-fvisibility=hidden)
endif()

if (ANDROID AND SHINY_VERBOSE_ANDROID_LOGGING)
  add_definitions(-DSHINY_VERBOSE_ANDROID_LOGGING=1)
endif()

add_library(Shiny STATIC ${SHINY_SOURCES} ${SHINY_HEADERS})

if (ANDROID)
  target_link_libraries(Shiny PUBLIC log)
endif()

# define variable for legacy usages
set(SHINY_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(Shiny PUBLIC ${SHINY_INCLUDE_DIR})

if (MSVC)
    set(COMPILE_OPTIONS /W4)
else()
    set(COMPILE_OPTIONS -Wall)
endif()

if(SHINY_IS_COMPILED)
    add_definitions(-DSHINY_IS_COMPILED=TRUE)
endif()

if(SHINY_HAS_ENABLED)
    add_definitions(-DSHINY_HAS_ENABLED=TRUE)
endif()

target_compile_options(Shiny PRIVATE ${COMPILE_OPTIONS})

set(SHINY_TARGET_PROPERTIES
  POSITION_INDEPENDENT_CODE YES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED YES)

get_target_property(SHINY_LIBRARY_TYPE Shiny TYPE)
if (${SHINY_LIBRARY_TYPE} STREQUAL STATIC_LIBRARY)
    set(LIBRARY_SUFFIX -static)
endif()

set_target_properties(
    Shiny
    PROPERTIES
    ${SHINY_TARGET_PROPERTIES}
    OUTPUT_NAME Shiny${LIBRARY_SUFFIX})

if (SHINY_BUILD_SAMPLES)
    if (SHINY_HOSTED_BUILD)
        message(WARNING "Shiny Samples: Compilation errors may occur when Shiny is hosted")
    endif()

    add_subdirectory(Samples)
    add_custom_target(ShinySamples ALL)

    add_dependencies(ShinySamples
        ShinySimpleSample
        ShinyMainLoopSample
        ShinyAdvancedSample
        ShinyThreadSample)
endif()

install(TARGETS Shiny
    RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
    ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
    LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )

install(FILES ${SHINY_HEADERS} DESTINATION "${INSTALL_INC_DIR}")
# add_definitions()
