# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/build

# Include any dependencies generated for this target.
include Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/depend.make

# Include the progress variables for this target.
include Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/progress.make

# Include the compile flags for this target's objects.
include Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/flags.make

Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/ThreadSample.cpp.o: Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/flags.make
Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/ThreadSample.cpp.o: ../Samples/ThreadSample/ThreadSample.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/ThreadSample.cpp.o"
	cd /home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/build/Samples/ThreadSample && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ShinyThreadSample.dir/ThreadSample.cpp.o -c /home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/Samples/ThreadSample/ThreadSample.cpp

Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/ThreadSample.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ShinyThreadSample.dir/ThreadSample.cpp.i"
	cd /home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/build/Samples/ThreadSample && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/Samples/ThreadSample/ThreadSample.cpp > CMakeFiles/ShinyThreadSample.dir/ThreadSample.cpp.i

Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/ThreadSample.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ShinyThreadSample.dir/ThreadSample.cpp.s"
	cd /home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/build/Samples/ThreadSample && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/Samples/ThreadSample/ThreadSample.cpp -o CMakeFiles/ShinyThreadSample.dir/ThreadSample.cpp.s

Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/ThreadSample.cpp.o.requires:

.PHONY : Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/ThreadSample.cpp.o.requires

Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/ThreadSample.cpp.o.provides: Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/ThreadSample.cpp.o.requires
	$(MAKE) -f Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/build.make Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/ThreadSample.cpp.o.provides.build
.PHONY : Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/ThreadSample.cpp.o.provides

Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/ThreadSample.cpp.o.provides.build: Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/ThreadSample.cpp.o


# Object files for target ShinyThreadSample
ShinyThreadSample_OBJECTS = \
"CMakeFiles/ShinyThreadSample.dir/ThreadSample.cpp.o"

# External object files for target ShinyThreadSample
ShinyThreadSample_EXTERNAL_OBJECTS =

bin/ShinyThreadSample: Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/ThreadSample.cpp.o
bin/ShinyThreadSample: Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/build.make
bin/ShinyThreadSample: lib/libShiny-static.a
bin/ShinyThreadSample: Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/ShinyThreadSample"
	cd /home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/build/Samples/ThreadSample && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ShinyThreadSample.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/build: bin/ShinyThreadSample

.PHONY : Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/build

Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/requires: Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/ThreadSample.cpp.o.requires

.PHONY : Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/requires

Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/clean:
	cd /home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/build/Samples/ThreadSample && $(CMAKE_COMMAND) -P CMakeFiles/ShinyThreadSample.dir/cmake_clean.cmake
.PHONY : Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/clean

Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/depend:
	cd /home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler /home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/Samples/ThreadSample /home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/build /home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/build/Samples/ThreadSample /home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/build/Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Samples/ThreadSample/CMakeFiles/ShinyThreadSample.dir/depend

