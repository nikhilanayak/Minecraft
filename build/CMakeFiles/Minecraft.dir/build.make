# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/nikhil/Programming/Game

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nikhil/Programming/Game/build

# Include any dependencies generated for this target.
include CMakeFiles/Minecraft.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Minecraft.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Minecraft.dir/flags.make

CMakeFiles/Minecraft.dir/src/chunk.c.o: CMakeFiles/Minecraft.dir/flags.make
CMakeFiles/Minecraft.dir/src/chunk.c.o: ../src/chunk.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nikhil/Programming/Game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Minecraft.dir/src/chunk.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Minecraft.dir/src/chunk.c.o   -c /home/nikhil/Programming/Game/src/chunk.c

CMakeFiles/Minecraft.dir/src/chunk.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Minecraft.dir/src/chunk.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/nikhil/Programming/Game/src/chunk.c > CMakeFiles/Minecraft.dir/src/chunk.c.i

CMakeFiles/Minecraft.dir/src/chunk.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Minecraft.dir/src/chunk.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/nikhil/Programming/Game/src/chunk.c -o CMakeFiles/Minecraft.dir/src/chunk.c.s

CMakeFiles/Minecraft.dir/src/gl_vert.c.o: CMakeFiles/Minecraft.dir/flags.make
CMakeFiles/Minecraft.dir/src/gl_vert.c.o: ../src/gl_vert.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nikhil/Programming/Game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Minecraft.dir/src/gl_vert.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Minecraft.dir/src/gl_vert.c.o   -c /home/nikhil/Programming/Game/src/gl_vert.c

CMakeFiles/Minecraft.dir/src/gl_vert.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Minecraft.dir/src/gl_vert.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/nikhil/Programming/Game/src/gl_vert.c > CMakeFiles/Minecraft.dir/src/gl_vert.c.i

CMakeFiles/Minecraft.dir/src/gl_vert.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Minecraft.dir/src/gl_vert.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/nikhil/Programming/Game/src/gl_vert.c -o CMakeFiles/Minecraft.dir/src/gl_vert.c.s

CMakeFiles/Minecraft.dir/src/glad.c.o: CMakeFiles/Minecraft.dir/flags.make
CMakeFiles/Minecraft.dir/src/glad.c.o: ../src/glad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nikhil/Programming/Game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/Minecraft.dir/src/glad.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Minecraft.dir/src/glad.c.o   -c /home/nikhil/Programming/Game/src/glad.c

CMakeFiles/Minecraft.dir/src/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Minecraft.dir/src/glad.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/nikhil/Programming/Game/src/glad.c > CMakeFiles/Minecraft.dir/src/glad.c.i

CMakeFiles/Minecraft.dir/src/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Minecraft.dir/src/glad.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/nikhil/Programming/Game/src/glad.c -o CMakeFiles/Minecraft.dir/src/glad.c.s

CMakeFiles/Minecraft.dir/src/main.c.o: CMakeFiles/Minecraft.dir/flags.make
CMakeFiles/Minecraft.dir/src/main.c.o: ../src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nikhil/Programming/Game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/Minecraft.dir/src/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Minecraft.dir/src/main.c.o   -c /home/nikhil/Programming/Game/src/main.c

CMakeFiles/Minecraft.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Minecraft.dir/src/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/nikhil/Programming/Game/src/main.c > CMakeFiles/Minecraft.dir/src/main.c.i

CMakeFiles/Minecraft.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Minecraft.dir/src/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/nikhil/Programming/Game/src/main.c -o CMakeFiles/Minecraft.dir/src/main.c.s

CMakeFiles/Minecraft.dir/src/shader.c.o: CMakeFiles/Minecraft.dir/flags.make
CMakeFiles/Minecraft.dir/src/shader.c.o: ../src/shader.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nikhil/Programming/Game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/Minecraft.dir/src/shader.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Minecraft.dir/src/shader.c.o   -c /home/nikhil/Programming/Game/src/shader.c

CMakeFiles/Minecraft.dir/src/shader.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Minecraft.dir/src/shader.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/nikhil/Programming/Game/src/shader.c > CMakeFiles/Minecraft.dir/src/shader.c.i

CMakeFiles/Minecraft.dir/src/shader.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Minecraft.dir/src/shader.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/nikhil/Programming/Game/src/shader.c -o CMakeFiles/Minecraft.dir/src/shader.c.s

CMakeFiles/Minecraft.dir/src/vec.c.o: CMakeFiles/Minecraft.dir/flags.make
CMakeFiles/Minecraft.dir/src/vec.c.o: ../src/vec.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nikhil/Programming/Game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/Minecraft.dir/src/vec.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Minecraft.dir/src/vec.c.o   -c /home/nikhil/Programming/Game/src/vec.c

CMakeFiles/Minecraft.dir/src/vec.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Minecraft.dir/src/vec.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/nikhil/Programming/Game/src/vec.c > CMakeFiles/Minecraft.dir/src/vec.c.i

CMakeFiles/Minecraft.dir/src/vec.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Minecraft.dir/src/vec.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/nikhil/Programming/Game/src/vec.c -o CMakeFiles/Minecraft.dir/src/vec.c.s

# Object files for target Minecraft
Minecraft_OBJECTS = \
"CMakeFiles/Minecraft.dir/src/chunk.c.o" \
"CMakeFiles/Minecraft.dir/src/gl_vert.c.o" \
"CMakeFiles/Minecraft.dir/src/glad.c.o" \
"CMakeFiles/Minecraft.dir/src/main.c.o" \
"CMakeFiles/Minecraft.dir/src/shader.c.o" \
"CMakeFiles/Minecraft.dir/src/vec.c.o"

# External object files for target Minecraft
Minecraft_EXTERNAL_OBJECTS =

Minecraft: CMakeFiles/Minecraft.dir/src/chunk.c.o
Minecraft: CMakeFiles/Minecraft.dir/src/gl_vert.c.o
Minecraft: CMakeFiles/Minecraft.dir/src/glad.c.o
Minecraft: CMakeFiles/Minecraft.dir/src/main.c.o
Minecraft: CMakeFiles/Minecraft.dir/src/shader.c.o
Minecraft: CMakeFiles/Minecraft.dir/src/vec.c.o
Minecraft: CMakeFiles/Minecraft.dir/build.make
Minecraft: CMakeFiles/Minecraft.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nikhil/Programming/Game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking C executable Minecraft"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Minecraft.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Minecraft.dir/build: Minecraft

.PHONY : CMakeFiles/Minecraft.dir/build

CMakeFiles/Minecraft.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Minecraft.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Minecraft.dir/clean

CMakeFiles/Minecraft.dir/depend:
	cd /home/nikhil/Programming/Game/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nikhil/Programming/Game /home/nikhil/Programming/Game /home/nikhil/Programming/Game/build /home/nikhil/Programming/Game/build /home/nikhil/Programming/Game/build/CMakeFiles/Minecraft.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Minecraft.dir/depend

