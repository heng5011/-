cmake_minimum_required(VERSION 3.6)

project(Marc-TicToc LANGUAGES C CXX)
#安装好第三方库后可在CMake中使用find_package()引入依赖
find_package(Threads)

# c++标准
set(CMAKE_CXX_STANDARD 14)

set(SIMPLE_SOURCES 
# examples2/subpart.cpp
# examples2/subpart.h
# examples2/testtictoc.cpp
examples1/frs_test.cpp  
# examples3/test.cpp
src/tictoc.cpp
src/Timer.cpp
)

# 用来找头文件路径
include_directories(Marc-TicToc include)

add_executable(Marc-TicToc ${SIMPLE_SOURCES})

# 如果是android
if (ANDROID)
  target_link_libraries(Marc-TicToc PUBLIC log)
endif()

# 链接Thread库
# 使用target_link_libraries链接库时，需要在add_executable之后
target_link_libraries(Marc-TicToc ${CMAKE_THREAD_LIBS_INIT})

#target_include_directories(Marc-TicToc PUBLIC  ${CMAKE_CURRENT_LIST_DIR}/)