# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /snap/clion/69/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/69/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ilyshka/Desktop/Unik/SpoVM/lab_5

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ilyshka/Desktop/Unik/SpoVM/lab_5/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/AsyncIO.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/AsyncIO.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/AsyncIO.dir/flags.make

CMakeFiles/AsyncIO.dir/AsyncIO.cpp.o: CMakeFiles/AsyncIO.dir/flags.make
CMakeFiles/AsyncIO.dir/AsyncIO.cpp.o: ../AsyncIO.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ilyshka/Desktop/Unik/SpoVM/lab_5/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/AsyncIO.dir/AsyncIO.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/AsyncIO.dir/AsyncIO.cpp.o -c /home/ilyshka/Desktop/Unik/SpoVM/lab_5/AsyncIO.cpp

CMakeFiles/AsyncIO.dir/AsyncIO.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AsyncIO.dir/AsyncIO.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ilyshka/Desktop/Unik/SpoVM/lab_5/AsyncIO.cpp > CMakeFiles/AsyncIO.dir/AsyncIO.cpp.i

CMakeFiles/AsyncIO.dir/AsyncIO.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AsyncIO.dir/AsyncIO.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ilyshka/Desktop/Unik/SpoVM/lab_5/AsyncIO.cpp -o CMakeFiles/AsyncIO.dir/AsyncIO.cpp.s

# Object files for target AsyncIO
AsyncIO_OBJECTS = \
"CMakeFiles/AsyncIO.dir/AsyncIO.cpp.o"

# External object files for target AsyncIO
AsyncIO_EXTERNAL_OBJECTS =

libAsyncIO.so: CMakeFiles/AsyncIO.dir/AsyncIO.cpp.o
libAsyncIO.so: CMakeFiles/AsyncIO.dir/build.make
libAsyncIO.so: CMakeFiles/AsyncIO.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ilyshka/Desktop/Unik/SpoVM/lab_5/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libAsyncIO.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/AsyncIO.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/AsyncIO.dir/build: libAsyncIO.so

.PHONY : CMakeFiles/AsyncIO.dir/build

CMakeFiles/AsyncIO.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/AsyncIO.dir/cmake_clean.cmake
.PHONY : CMakeFiles/AsyncIO.dir/clean

CMakeFiles/AsyncIO.dir/depend:
	cd /home/ilyshka/Desktop/Unik/SpoVM/lab_5/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ilyshka/Desktop/Unik/SpoVM/lab_5 /home/ilyshka/Desktop/Unik/SpoVM/lab_5 /home/ilyshka/Desktop/Unik/SpoVM/lab_5/cmake-build-debug /home/ilyshka/Desktop/Unik/SpoVM/lab_5/cmake-build-debug /home/ilyshka/Desktop/Unik/SpoVM/lab_5/cmake-build-debug/CMakeFiles/AsyncIO.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/AsyncIO.dir/depend

