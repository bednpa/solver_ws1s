# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.24.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.24.1/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s/build

# Include any dependencies generated for this target.
include CMakeFiles/gencode.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/gencode.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/gencode.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/gencode.dir/flags.make

CMakeFiles/gencode.dir/src/core/gencode.cpp.o: CMakeFiles/gencode.dir/flags.make
CMakeFiles/gencode.dir/src/core/gencode.cpp.o: /Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s/src/core/gencode.cpp
CMakeFiles/gencode.dir/src/core/gencode.cpp.o: CMakeFiles/gencode.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/gencode.dir/src/core/gencode.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/gencode.dir/src/core/gencode.cpp.o -MF CMakeFiles/gencode.dir/src/core/gencode.cpp.o.d -o CMakeFiles/gencode.dir/src/core/gencode.cpp.o -c /Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s/src/core/gencode.cpp

CMakeFiles/gencode.dir/src/core/gencode.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gencode.dir/src/core/gencode.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s/src/core/gencode.cpp > CMakeFiles/gencode.dir/src/core/gencode.cpp.i

CMakeFiles/gencode.dir/src/core/gencode.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gencode.dir/src/core/gencode.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s/src/core/gencode.cpp -o CMakeFiles/gencode.dir/src/core/gencode.cpp.s

# Object files for target gencode
gencode_OBJECTS = \
"CMakeFiles/gencode.dir/src/core/gencode.cpp.o"

# External object files for target gencode
gencode_EXTERNAL_OBJECTS =

libgencode.a: CMakeFiles/gencode.dir/src/core/gencode.cpp.o
libgencode.a: CMakeFiles/gencode.dir/build.make
libgencode.a: CMakeFiles/gencode.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libgencode.a"
	$(CMAKE_COMMAND) -P CMakeFiles/gencode.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gencode.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/gencode.dir/build: libgencode.a
.PHONY : CMakeFiles/gencode.dir/build

CMakeFiles/gencode.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/gencode.dir/cmake_clean.cmake
.PHONY : CMakeFiles/gencode.dir/clean

CMakeFiles/gencode.dir/depend:
	cd /Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s /Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s /Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s/build /Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s/build /Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s/build/CMakeFiles/gencode.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/gencode.dir/depend

