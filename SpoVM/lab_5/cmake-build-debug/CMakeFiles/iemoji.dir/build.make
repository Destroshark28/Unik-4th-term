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
include CMakeFiles/iemoji.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/iemoji.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/iemoji.dir/flags.make

CMakeFiles/iemoji.dir/AsyncReadWrite.cpp.o: CMakeFiles/iemoji.dir/flags.make
CMakeFiles/iemoji.dir/AsyncReadWrite.cpp.o: ../AsyncReadWrite.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ilyshka/Desktop/Unik/SpoVM/lab_5/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/iemoji.dir/AsyncReadWrite.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/iemoji.dir/AsyncReadWrite.cpp.o -c /home/ilyshka/Desktop/Unik/SpoVM/lab_5/AsyncReadWrite.cpp

CMakeFiles/iemoji.dir/AsyncReadWrite.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/iemoji.dir/AsyncReadWrite.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ilyshka/Desktop/Unik/SpoVM/lab_5/AsyncReadWrite.cpp > CMakeFiles/iemoji.dir/AsyncReadWrite.cpp.i

CMakeFiles/iemoji.dir/AsyncReadWrite.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/iemoji.dir/AsyncReadWrite.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ilyshka/Desktop/Unik/SpoVM/lab_5/AsyncReadWrite.cpp -o CMakeFiles/iemoji.dir/AsyncReadWrite.cpp.s

# Object files for target iemoji
iemoji_OBJECTS = \
"CMakeFiles/iemoji.dir/AsyncReadWrite.cpp.o"

# External object files for target iemoji
iemoji_EXTERNAL_OBJECTS =

libiemoji.so: CMakeFiles/iemoji.dir/AsyncReadWrite.cpp.o
libiemoji.so: CMakeFiles/iemoji.dir/build.make
libiemoji.so: CMakeFiles/iemoji.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ilyshka/Desktop/Unik/SpoVM/lab_5/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libiemoji.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/iemoji.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/iemoji.dir/build: libiemoji.so

.PHONY : CMakeFiles/iemoji.dir/build

CMakeFiles/iemoji.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/iemoji.dir/cmake_clean.cmake
.PHONY : CMakeFiles/iemoji.dir/clean

CMakeFiles/iemoji.dir/depend:
	cd /home/ilyshka/Desktop/Unik/SpoVM/lab_5/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ilyshka/Desktop/Unik/SpoVM/lab_5 /home/ilyshka/Desktop/Unik/SpoVM/lab_5 /home/ilyshka/Desktop/Unik/SpoVM/lab_5/cmake-build-debug /home/ilyshka/Desktop/Unik/SpoVM/lab_5/cmake-build-debug /home/ilyshka/Desktop/Unik/SpoVM/lab_5/cmake-build-debug/CMakeFiles/iemoji.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/iemoji.dir/depend

