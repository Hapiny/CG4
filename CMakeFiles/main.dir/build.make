# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = "/home/artem/5sem/Computer Graphic/TASK 4"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/artem/5sem/Computer Graphic/TASK 4"

# Include any dependencies generated for this target.
include CMakeFiles/main.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/main.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/main.dir/flags.make

CMakeFiles/main.dir/glad.c.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/glad.c.o: glad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/artem/5sem/Computer Graphic/TASK 4/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/main.dir/glad.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/main.dir/glad.c.o   -c "/home/artem/5sem/Computer Graphic/TASK 4/glad.c"

CMakeFiles/main.dir/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/main.dir/glad.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/home/artem/5sem/Computer Graphic/TASK 4/glad.c" > CMakeFiles/main.dir/glad.c.i

CMakeFiles/main.dir/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/main.dir/glad.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/home/artem/5sem/Computer Graphic/TASK 4/glad.c" -o CMakeFiles/main.dir/glad.c.s

CMakeFiles/main.dir/glad.c.o.requires:

.PHONY : CMakeFiles/main.dir/glad.c.o.requires

CMakeFiles/main.dir/glad.c.o.provides: CMakeFiles/main.dir/glad.c.o.requires
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/glad.c.o.provides.build
.PHONY : CMakeFiles/main.dir/glad.c.o.provides

CMakeFiles/main.dir/glad.c.o.provides.build: CMakeFiles/main.dir/glad.c.o


CMakeFiles/main.dir/main.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/artem/5sem/Computer Graphic/TASK 4/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/main.dir/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/main.cpp.o -c "/home/artem/5sem/Computer Graphic/TASK 4/main.cpp"

CMakeFiles/main.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/artem/5sem/Computer Graphic/TASK 4/main.cpp" > CMakeFiles/main.dir/main.cpp.i

CMakeFiles/main.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/artem/5sem/Computer Graphic/TASK 4/main.cpp" -o CMakeFiles/main.dir/main.cpp.s

CMakeFiles/main.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/main.dir/main.cpp.o.requires

CMakeFiles/main.dir/main.cpp.o.provides: CMakeFiles/main.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/main.dir/main.cpp.o.provides

CMakeFiles/main.dir/main.cpp.o.provides.build: CMakeFiles/main.dir/main.cpp.o


CMakeFiles/main.dir/Camera.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/Camera.cpp.o: Camera.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/artem/5sem/Computer Graphic/TASK 4/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/main.dir/Camera.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/Camera.cpp.o -c "/home/artem/5sem/Computer Graphic/TASK 4/Camera.cpp"

CMakeFiles/main.dir/Camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/Camera.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/artem/5sem/Computer Graphic/TASK 4/Camera.cpp" > CMakeFiles/main.dir/Camera.cpp.i

CMakeFiles/main.dir/Camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/Camera.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/artem/5sem/Computer Graphic/TASK 4/Camera.cpp" -o CMakeFiles/main.dir/Camera.cpp.s

CMakeFiles/main.dir/Camera.cpp.o.requires:

.PHONY : CMakeFiles/main.dir/Camera.cpp.o.requires

CMakeFiles/main.dir/Camera.cpp.o.provides: CMakeFiles/main.dir/Camera.cpp.o.requires
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/Camera.cpp.o.provides.build
.PHONY : CMakeFiles/main.dir/Camera.cpp.o.provides

CMakeFiles/main.dir/Camera.cpp.o.provides.build: CMakeFiles/main.dir/Camera.cpp.o


CMakeFiles/main.dir/ShaderProgram.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/ShaderProgram.cpp.o: ShaderProgram.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/artem/5sem/Computer Graphic/TASK 4/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/main.dir/ShaderProgram.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/ShaderProgram.cpp.o -c "/home/artem/5sem/Computer Graphic/TASK 4/ShaderProgram.cpp"

CMakeFiles/main.dir/ShaderProgram.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/ShaderProgram.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/artem/5sem/Computer Graphic/TASK 4/ShaderProgram.cpp" > CMakeFiles/main.dir/ShaderProgram.cpp.i

CMakeFiles/main.dir/ShaderProgram.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/ShaderProgram.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/artem/5sem/Computer Graphic/TASK 4/ShaderProgram.cpp" -o CMakeFiles/main.dir/ShaderProgram.cpp.s

CMakeFiles/main.dir/ShaderProgram.cpp.o.requires:

.PHONY : CMakeFiles/main.dir/ShaderProgram.cpp.o.requires

CMakeFiles/main.dir/ShaderProgram.cpp.o.provides: CMakeFiles/main.dir/ShaderProgram.cpp.o.requires
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/ShaderProgram.cpp.o.provides.build
.PHONY : CMakeFiles/main.dir/ShaderProgram.cpp.o.provides

CMakeFiles/main.dir/ShaderProgram.cpp.o.provides.build: CMakeFiles/main.dir/ShaderProgram.cpp.o


CMakeFiles/main.dir/Mesh.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/Mesh.cpp.o: Mesh.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/artem/5sem/Computer Graphic/TASK 4/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/main.dir/Mesh.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/Mesh.cpp.o -c "/home/artem/5sem/Computer Graphic/TASK 4/Mesh.cpp"

CMakeFiles/main.dir/Mesh.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/Mesh.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/artem/5sem/Computer Graphic/TASK 4/Mesh.cpp" > CMakeFiles/main.dir/Mesh.cpp.i

CMakeFiles/main.dir/Mesh.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/Mesh.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/artem/5sem/Computer Graphic/TASK 4/Mesh.cpp" -o CMakeFiles/main.dir/Mesh.cpp.s

CMakeFiles/main.dir/Mesh.cpp.o.requires:

.PHONY : CMakeFiles/main.dir/Mesh.cpp.o.requires

CMakeFiles/main.dir/Mesh.cpp.o.provides: CMakeFiles/main.dir/Mesh.cpp.o.requires
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/Mesh.cpp.o.provides.build
.PHONY : CMakeFiles/main.dir/Mesh.cpp.o.provides

CMakeFiles/main.dir/Mesh.cpp.o.provides.build: CMakeFiles/main.dir/Mesh.cpp.o


CMakeFiles/main.dir/Model.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/Model.cpp.o: Model.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/artem/5sem/Computer Graphic/TASK 4/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/main.dir/Model.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/Model.cpp.o -c "/home/artem/5sem/Computer Graphic/TASK 4/Model.cpp"

CMakeFiles/main.dir/Model.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/Model.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/artem/5sem/Computer Graphic/TASK 4/Model.cpp" > CMakeFiles/main.dir/Model.cpp.i

CMakeFiles/main.dir/Model.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/Model.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/artem/5sem/Computer Graphic/TASK 4/Model.cpp" -o CMakeFiles/main.dir/Model.cpp.s

CMakeFiles/main.dir/Model.cpp.o.requires:

.PHONY : CMakeFiles/main.dir/Model.cpp.o.requires

CMakeFiles/main.dir/Model.cpp.o.provides: CMakeFiles/main.dir/Model.cpp.o.requires
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/Model.cpp.o.provides.build
.PHONY : CMakeFiles/main.dir/Model.cpp.o.provides

CMakeFiles/main.dir/Model.cpp.o.provides.build: CMakeFiles/main.dir/Model.cpp.o


# Object files for target main
main_OBJECTS = \
"CMakeFiles/main.dir/glad.c.o" \
"CMakeFiles/main.dir/main.cpp.o" \
"CMakeFiles/main.dir/Camera.cpp.o" \
"CMakeFiles/main.dir/ShaderProgram.cpp.o" \
"CMakeFiles/main.dir/Mesh.cpp.o" \
"CMakeFiles/main.dir/Model.cpp.o"

# External object files for target main
main_EXTERNAL_OBJECTS =

main: CMakeFiles/main.dir/glad.c.o
main: CMakeFiles/main.dir/main.cpp.o
main: CMakeFiles/main.dir/Camera.cpp.o
main: CMakeFiles/main.dir/ShaderProgram.cpp.o
main: CMakeFiles/main.dir/Mesh.cpp.o
main: CMakeFiles/main.dir/Model.cpp.o
main: CMakeFiles/main.dir/build.make
main: /usr/lib/x86_64-linux-gnu/libGL.so
main: CMakeFiles/main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/artem/5sem/Computer Graphic/TASK 4/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable main"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main.dir/link.txt --verbose=$(VERBOSE)
	/usr/bin/cmake -E copy_directory /home/artem/5sem/Computer\ Graphic/TASK\ 4/shaders /home/artem/5sem/Computer\ Graphic/TASK\ 4

# Rule to build all files generated by this target.
CMakeFiles/main.dir/build: main

.PHONY : CMakeFiles/main.dir/build

CMakeFiles/main.dir/requires: CMakeFiles/main.dir/glad.c.o.requires
CMakeFiles/main.dir/requires: CMakeFiles/main.dir/main.cpp.o.requires
CMakeFiles/main.dir/requires: CMakeFiles/main.dir/Camera.cpp.o.requires
CMakeFiles/main.dir/requires: CMakeFiles/main.dir/ShaderProgram.cpp.o.requires
CMakeFiles/main.dir/requires: CMakeFiles/main.dir/Mesh.cpp.o.requires
CMakeFiles/main.dir/requires: CMakeFiles/main.dir/Model.cpp.o.requires

.PHONY : CMakeFiles/main.dir/requires

CMakeFiles/main.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/main.dir/cmake_clean.cmake
.PHONY : CMakeFiles/main.dir/clean

CMakeFiles/main.dir/depend:
	cd "/home/artem/5sem/Computer Graphic/TASK 4" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/artem/5sem/Computer Graphic/TASK 4" "/home/artem/5sem/Computer Graphic/TASK 4" "/home/artem/5sem/Computer Graphic/TASK 4" "/home/artem/5sem/Computer Graphic/TASK 4" "/home/artem/5sem/Computer Graphic/TASK 4/CMakeFiles/main.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/main.dir/depend

