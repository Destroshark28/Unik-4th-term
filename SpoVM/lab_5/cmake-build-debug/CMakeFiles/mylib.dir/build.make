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
include CMakeFiles/mylib.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/mylib.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mylib.dir/flags.make

CMakeFiles/mylib.dir/asyncio/AsyncReadWrite.cpp.o: CMakeFiles/mylib.dir/flags.make
CMakeFiles/mylib.dir/asyncio/AsyncReadWrite.cpp.o: ../asyncio/AsyncReadWrite.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ilyshka/Desktop/Unik/SpoVM/lab_5/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/mylib.dir/asyncio/AsyncReadWrite.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mylib.dir/asyncio/AsyncReadWrite.cpp.o -c /home/ilyshka/Desktop/Unik/SpoVM/lab_5/asyncio/AsyncReadWrite.cpp

CMakeFiles/mylib.dir/asyncio/AsyncReadWrite.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mylib.dir/asyncio/AsyncReadWrite.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ilyshka/Desktop/Unik/SpoVM/lab_5/asyncio/AsyncReadWrite.cpp > CMakeFiles/mylib.dir/asyncio/AsyncReadWrite.cpp.i

CMakeFiles/mylib.dir/asyncio/AsyncReadWrite.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mylib.dir/asyncio/AsyncReadWrite.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ilyshka/Desktop/Unik/SpoVM/lab_5/asyncio/AsyncReadWrite.cpp -o CMakeFiles/mylib.dir/asyncio/AsyncReadWrite.cpp.s

# Object files for target mylib
mylib_OBJECTS = \
"CMakeFiles/mylib.dir/asyncio/AsyncReadWrite.cpp.o"

# External object files for target mylib
mylib_EXTERNAL_OBJECTS =

libmylib.so: CMakeFiles/mylib.dir/asyncio/AsyncReadWrite.cpp.o
libmylib.so: CMakeFiles/mylib.dir/build.make
libmylib.so: CMakeFiles/mylib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ilyshka/Desktop/Unik/SpoVM/lab_5/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libmylib.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mylib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mylib.dir/build: libmylib.so

.PHONY : CMakeFiles/mylib.dir/build

CMakeFiles/mylib.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mylib.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mylib.dir/clean

CMakeFiles/mylib.dir/depend:
	cd /home/ilyshka/Desktop/Unik/SpoVM/lab_5/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ilyshka/Desktop/Unik/SpoVM/lab_5 /home/ilyshka/Desktop/Unik/SpoVM/lab_5 /home/ilyshka/Desktop/Unik/SpoVM/lab_5/cmake-build-debug /home/ilyshka/Desktop/Unik/SpoVM/lab_5/cmake-build-debug /home/ilyshka/Desktop/Unik/SpoVM/lab_5/cmake-build-debug/CMakeFiles/mylib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mylib.dir/depend

