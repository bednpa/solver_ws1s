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
include CMakeFiles/atomaut.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/atomaut.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/atomaut.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/atomaut.dir/flags.make

CMakeFiles/atomaut.dir/src/core/atomaut.cpp.o: CMakeFiles/atomaut.dir/flags.make
CMakeFiles/atomaut.dir/src/core/atomaut.cpp.o: /Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s/src/core/atomaut.cpp
CMakeFiles/atomaut.dir/src/core/atomaut.cpp.o: CMakeFiles/atomaut.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/atomaut.dir/src/core/atomaut.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/atomaut.dir/src/core/atomaut.cpp.o -MF CMakeFiles/atomaut.dir/src/core/atomaut.cpp.o.d -o CMakeFiles/atomaut.dir/src/core/atomaut.cpp.o -c /Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s/src/core/atomaut.cpp

CMakeFiles/atomaut.dir/src/core/atomaut.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/atomaut.dir/src/core/atomaut.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s/src/core/atomaut.cpp > CMakeFiles/atomaut.dir/src/core/atomaut.cpp.i

CMakeFiles/atomaut.dir/src/core/atomaut.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/atomaut.dir/src/core/atomaut.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s/src/core/atomaut.cpp -o CMakeFiles/atomaut.dir/src/core/atomaut.cpp.s

# Object files for target atomaut
atomaut_OBJECTS = \
"CMakeFiles/atomaut.dir/src/core/atomaut.cpp.o"

# External object files for target atomaut
atomaut_EXTERNAL_OBJECTS =

libatomaut.a: CMakeFiles/atomaut.dir/src/core/atomaut.cpp.o
libatomaut.a: CMakeFiles/atomaut.dir/build.make
libatomaut.a: CMakeFiles/atomaut.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libatomaut.a"
	$(CMAKE_COMMAND) -P CMakeFiles/atomaut.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/atomaut.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/atomaut.dir/build: libatomaut.a
.PHONY : CMakeFiles/atomaut.dir/build

CMakeFiles/atomaut.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/atomaut.dir/cmake_clean.cmake
.PHONY : CMakeFiles/atomaut.dir/clean

CMakeFiles/atomaut.dir/depend:
	cd /Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s /Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s /Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s/build /Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s/build /Users/pavelbednar/Documents/FIT/NMAT/4._semestr/DIP/solver_ws1s/build/CMakeFiles/atomaut.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/atomaut.dir/depend

