# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /host_WorkDir/ldd_work/c++_work/tstMuduo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /host_WorkDir/ldd_work/c++_work/tstMuduo/build

# Include any dependencies generated for this target.
include muduo/base/CMakeFiles/muduo_base.dir/depend.make

# Include the progress variables for this target.
include muduo/base/CMakeFiles/muduo_base.dir/progress.make

# Include the compile flags for this target's objects.
include muduo/base/CMakeFiles/muduo_base.dir/flags.make

muduo/base/CMakeFiles/muduo_base.dir/Condition.cc.o: muduo/base/CMakeFiles/muduo_base.dir/flags.make
muduo/base/CMakeFiles/muduo_base.dir/Condition.cc.o: ../muduo/base/Condition.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/host_WorkDir/ldd_work/c++_work/tstMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object muduo/base/CMakeFiles/muduo_base.dir/Condition.cc.o"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/muduo_base.dir/Condition.cc.o -c /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/Condition.cc

muduo/base/CMakeFiles/muduo_base.dir/Condition.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/muduo_base.dir/Condition.cc.i"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/Condition.cc > CMakeFiles/muduo_base.dir/Condition.cc.i

muduo/base/CMakeFiles/muduo_base.dir/Condition.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/muduo_base.dir/Condition.cc.s"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/Condition.cc -o CMakeFiles/muduo_base.dir/Condition.cc.s

muduo/base/CMakeFiles/muduo_base.dir/Condition.cc.o.requires:

.PHONY : muduo/base/CMakeFiles/muduo_base.dir/Condition.cc.o.requires

muduo/base/CMakeFiles/muduo_base.dir/Condition.cc.o.provides: muduo/base/CMakeFiles/muduo_base.dir/Condition.cc.o.requires
	$(MAKE) -f muduo/base/CMakeFiles/muduo_base.dir/build.make muduo/base/CMakeFiles/muduo_base.dir/Condition.cc.o.provides.build
.PHONY : muduo/base/CMakeFiles/muduo_base.dir/Condition.cc.o.provides

muduo/base/CMakeFiles/muduo_base.dir/Condition.cc.o.provides.build: muduo/base/CMakeFiles/muduo_base.dir/Condition.cc.o


muduo/base/CMakeFiles/muduo_base.dir/CountDownLatch.cc.o: muduo/base/CMakeFiles/muduo_base.dir/flags.make
muduo/base/CMakeFiles/muduo_base.dir/CountDownLatch.cc.o: ../muduo/base/CountDownLatch.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/host_WorkDir/ldd_work/c++_work/tstMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object muduo/base/CMakeFiles/muduo_base.dir/CountDownLatch.cc.o"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/muduo_base.dir/CountDownLatch.cc.o -c /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/CountDownLatch.cc

muduo/base/CMakeFiles/muduo_base.dir/CountDownLatch.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/muduo_base.dir/CountDownLatch.cc.i"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/CountDownLatch.cc > CMakeFiles/muduo_base.dir/CountDownLatch.cc.i

muduo/base/CMakeFiles/muduo_base.dir/CountDownLatch.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/muduo_base.dir/CountDownLatch.cc.s"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/CountDownLatch.cc -o CMakeFiles/muduo_base.dir/CountDownLatch.cc.s

muduo/base/CMakeFiles/muduo_base.dir/CountDownLatch.cc.o.requires:

.PHONY : muduo/base/CMakeFiles/muduo_base.dir/CountDownLatch.cc.o.requires

muduo/base/CMakeFiles/muduo_base.dir/CountDownLatch.cc.o.provides: muduo/base/CMakeFiles/muduo_base.dir/CountDownLatch.cc.o.requires
	$(MAKE) -f muduo/base/CMakeFiles/muduo_base.dir/build.make muduo/base/CMakeFiles/muduo_base.dir/CountDownLatch.cc.o.provides.build
.PHONY : muduo/base/CMakeFiles/muduo_base.dir/CountDownLatch.cc.o.provides

muduo/base/CMakeFiles/muduo_base.dir/CountDownLatch.cc.o.provides.build: muduo/base/CMakeFiles/muduo_base.dir/CountDownLatch.cc.o


muduo/base/CMakeFiles/muduo_base.dir/Date.cc.o: muduo/base/CMakeFiles/muduo_base.dir/flags.make
muduo/base/CMakeFiles/muduo_base.dir/Date.cc.o: ../muduo/base/Date.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/host_WorkDir/ldd_work/c++_work/tstMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object muduo/base/CMakeFiles/muduo_base.dir/Date.cc.o"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/muduo_base.dir/Date.cc.o -c /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/Date.cc

muduo/base/CMakeFiles/muduo_base.dir/Date.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/muduo_base.dir/Date.cc.i"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/Date.cc > CMakeFiles/muduo_base.dir/Date.cc.i

muduo/base/CMakeFiles/muduo_base.dir/Date.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/muduo_base.dir/Date.cc.s"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/Date.cc -o CMakeFiles/muduo_base.dir/Date.cc.s

muduo/base/CMakeFiles/muduo_base.dir/Date.cc.o.requires:

.PHONY : muduo/base/CMakeFiles/muduo_base.dir/Date.cc.o.requires

muduo/base/CMakeFiles/muduo_base.dir/Date.cc.o.provides: muduo/base/CMakeFiles/muduo_base.dir/Date.cc.o.requires
	$(MAKE) -f muduo/base/CMakeFiles/muduo_base.dir/build.make muduo/base/CMakeFiles/muduo_base.dir/Date.cc.o.provides.build
.PHONY : muduo/base/CMakeFiles/muduo_base.dir/Date.cc.o.provides

muduo/base/CMakeFiles/muduo_base.dir/Date.cc.o.provides.build: muduo/base/CMakeFiles/muduo_base.dir/Date.cc.o


muduo/base/CMakeFiles/muduo_base.dir/Exception.cc.o: muduo/base/CMakeFiles/muduo_base.dir/flags.make
muduo/base/CMakeFiles/muduo_base.dir/Exception.cc.o: ../muduo/base/Exception.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/host_WorkDir/ldd_work/c++_work/tstMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object muduo/base/CMakeFiles/muduo_base.dir/Exception.cc.o"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/muduo_base.dir/Exception.cc.o -c /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/Exception.cc

muduo/base/CMakeFiles/muduo_base.dir/Exception.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/muduo_base.dir/Exception.cc.i"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/Exception.cc > CMakeFiles/muduo_base.dir/Exception.cc.i

muduo/base/CMakeFiles/muduo_base.dir/Exception.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/muduo_base.dir/Exception.cc.s"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/Exception.cc -o CMakeFiles/muduo_base.dir/Exception.cc.s

muduo/base/CMakeFiles/muduo_base.dir/Exception.cc.o.requires:

.PHONY : muduo/base/CMakeFiles/muduo_base.dir/Exception.cc.o.requires

muduo/base/CMakeFiles/muduo_base.dir/Exception.cc.o.provides: muduo/base/CMakeFiles/muduo_base.dir/Exception.cc.o.requires
	$(MAKE) -f muduo/base/CMakeFiles/muduo_base.dir/build.make muduo/base/CMakeFiles/muduo_base.dir/Exception.cc.o.provides.build
.PHONY : muduo/base/CMakeFiles/muduo_base.dir/Exception.cc.o.provides

muduo/base/CMakeFiles/muduo_base.dir/Exception.cc.o.provides.build: muduo/base/CMakeFiles/muduo_base.dir/Exception.cc.o


muduo/base/CMakeFiles/muduo_base.dir/Logging.cc.o: muduo/base/CMakeFiles/muduo_base.dir/flags.make
muduo/base/CMakeFiles/muduo_base.dir/Logging.cc.o: ../muduo/base/Logging.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/host_WorkDir/ldd_work/c++_work/tstMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object muduo/base/CMakeFiles/muduo_base.dir/Logging.cc.o"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/muduo_base.dir/Logging.cc.o -c /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/Logging.cc

muduo/base/CMakeFiles/muduo_base.dir/Logging.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/muduo_base.dir/Logging.cc.i"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/Logging.cc > CMakeFiles/muduo_base.dir/Logging.cc.i

muduo/base/CMakeFiles/muduo_base.dir/Logging.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/muduo_base.dir/Logging.cc.s"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/Logging.cc -o CMakeFiles/muduo_base.dir/Logging.cc.s

muduo/base/CMakeFiles/muduo_base.dir/Logging.cc.o.requires:

.PHONY : muduo/base/CMakeFiles/muduo_base.dir/Logging.cc.o.requires

muduo/base/CMakeFiles/muduo_base.dir/Logging.cc.o.provides: muduo/base/CMakeFiles/muduo_base.dir/Logging.cc.o.requires
	$(MAKE) -f muduo/base/CMakeFiles/muduo_base.dir/build.make muduo/base/CMakeFiles/muduo_base.dir/Logging.cc.o.provides.build
.PHONY : muduo/base/CMakeFiles/muduo_base.dir/Logging.cc.o.provides

muduo/base/CMakeFiles/muduo_base.dir/Logging.cc.o.provides.build: muduo/base/CMakeFiles/muduo_base.dir/Logging.cc.o


muduo/base/CMakeFiles/muduo_base.dir/LogStream.cc.o: muduo/base/CMakeFiles/muduo_base.dir/flags.make
muduo/base/CMakeFiles/muduo_base.dir/LogStream.cc.o: ../muduo/base/LogStream.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/host_WorkDir/ldd_work/c++_work/tstMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object muduo/base/CMakeFiles/muduo_base.dir/LogStream.cc.o"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/muduo_base.dir/LogStream.cc.o -c /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/LogStream.cc

muduo/base/CMakeFiles/muduo_base.dir/LogStream.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/muduo_base.dir/LogStream.cc.i"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/LogStream.cc > CMakeFiles/muduo_base.dir/LogStream.cc.i

muduo/base/CMakeFiles/muduo_base.dir/LogStream.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/muduo_base.dir/LogStream.cc.s"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/LogStream.cc -o CMakeFiles/muduo_base.dir/LogStream.cc.s

muduo/base/CMakeFiles/muduo_base.dir/LogStream.cc.o.requires:

.PHONY : muduo/base/CMakeFiles/muduo_base.dir/LogStream.cc.o.requires

muduo/base/CMakeFiles/muduo_base.dir/LogStream.cc.o.provides: muduo/base/CMakeFiles/muduo_base.dir/LogStream.cc.o.requires
	$(MAKE) -f muduo/base/CMakeFiles/muduo_base.dir/build.make muduo/base/CMakeFiles/muduo_base.dir/LogStream.cc.o.provides.build
.PHONY : muduo/base/CMakeFiles/muduo_base.dir/LogStream.cc.o.provides

muduo/base/CMakeFiles/muduo_base.dir/LogStream.cc.o.provides.build: muduo/base/CMakeFiles/muduo_base.dir/LogStream.cc.o


muduo/base/CMakeFiles/muduo_base.dir/Timestamp.cc.o: muduo/base/CMakeFiles/muduo_base.dir/flags.make
muduo/base/CMakeFiles/muduo_base.dir/Timestamp.cc.o: ../muduo/base/Timestamp.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/host_WorkDir/ldd_work/c++_work/tstMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object muduo/base/CMakeFiles/muduo_base.dir/Timestamp.cc.o"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/muduo_base.dir/Timestamp.cc.o -c /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/Timestamp.cc

muduo/base/CMakeFiles/muduo_base.dir/Timestamp.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/muduo_base.dir/Timestamp.cc.i"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/Timestamp.cc > CMakeFiles/muduo_base.dir/Timestamp.cc.i

muduo/base/CMakeFiles/muduo_base.dir/Timestamp.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/muduo_base.dir/Timestamp.cc.s"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/Timestamp.cc -o CMakeFiles/muduo_base.dir/Timestamp.cc.s

muduo/base/CMakeFiles/muduo_base.dir/Timestamp.cc.o.requires:

.PHONY : muduo/base/CMakeFiles/muduo_base.dir/Timestamp.cc.o.requires

muduo/base/CMakeFiles/muduo_base.dir/Timestamp.cc.o.provides: muduo/base/CMakeFiles/muduo_base.dir/Timestamp.cc.o.requires
	$(MAKE) -f muduo/base/CMakeFiles/muduo_base.dir/build.make muduo/base/CMakeFiles/muduo_base.dir/Timestamp.cc.o.provides.build
.PHONY : muduo/base/CMakeFiles/muduo_base.dir/Timestamp.cc.o.provides

muduo/base/CMakeFiles/muduo_base.dir/Timestamp.cc.o.provides.build: muduo/base/CMakeFiles/muduo_base.dir/Timestamp.cc.o


muduo/base/CMakeFiles/muduo_base.dir/TimeZone.cc.o: muduo/base/CMakeFiles/muduo_base.dir/flags.make
muduo/base/CMakeFiles/muduo_base.dir/TimeZone.cc.o: ../muduo/base/TimeZone.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/host_WorkDir/ldd_work/c++_work/tstMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object muduo/base/CMakeFiles/muduo_base.dir/TimeZone.cc.o"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/muduo_base.dir/TimeZone.cc.o -c /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/TimeZone.cc

muduo/base/CMakeFiles/muduo_base.dir/TimeZone.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/muduo_base.dir/TimeZone.cc.i"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/TimeZone.cc > CMakeFiles/muduo_base.dir/TimeZone.cc.i

muduo/base/CMakeFiles/muduo_base.dir/TimeZone.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/muduo_base.dir/TimeZone.cc.s"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/TimeZone.cc -o CMakeFiles/muduo_base.dir/TimeZone.cc.s

muduo/base/CMakeFiles/muduo_base.dir/TimeZone.cc.o.requires:

.PHONY : muduo/base/CMakeFiles/muduo_base.dir/TimeZone.cc.o.requires

muduo/base/CMakeFiles/muduo_base.dir/TimeZone.cc.o.provides: muduo/base/CMakeFiles/muduo_base.dir/TimeZone.cc.o.requires
	$(MAKE) -f muduo/base/CMakeFiles/muduo_base.dir/build.make muduo/base/CMakeFiles/muduo_base.dir/TimeZone.cc.o.provides.build
.PHONY : muduo/base/CMakeFiles/muduo_base.dir/TimeZone.cc.o.provides

muduo/base/CMakeFiles/muduo_base.dir/TimeZone.cc.o.provides.build: muduo/base/CMakeFiles/muduo_base.dir/TimeZone.cc.o


muduo/base/CMakeFiles/muduo_base.dir/Thread.cc.o: muduo/base/CMakeFiles/muduo_base.dir/flags.make
muduo/base/CMakeFiles/muduo_base.dir/Thread.cc.o: ../muduo/base/Thread.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/host_WorkDir/ldd_work/c++_work/tstMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object muduo/base/CMakeFiles/muduo_base.dir/Thread.cc.o"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/muduo_base.dir/Thread.cc.o -c /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/Thread.cc

muduo/base/CMakeFiles/muduo_base.dir/Thread.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/muduo_base.dir/Thread.cc.i"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/Thread.cc > CMakeFiles/muduo_base.dir/Thread.cc.i

muduo/base/CMakeFiles/muduo_base.dir/Thread.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/muduo_base.dir/Thread.cc.s"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/Thread.cc -o CMakeFiles/muduo_base.dir/Thread.cc.s

muduo/base/CMakeFiles/muduo_base.dir/Thread.cc.o.requires:

.PHONY : muduo/base/CMakeFiles/muduo_base.dir/Thread.cc.o.requires

muduo/base/CMakeFiles/muduo_base.dir/Thread.cc.o.provides: muduo/base/CMakeFiles/muduo_base.dir/Thread.cc.o.requires
	$(MAKE) -f muduo/base/CMakeFiles/muduo_base.dir/build.make muduo/base/CMakeFiles/muduo_base.dir/Thread.cc.o.provides.build
.PHONY : muduo/base/CMakeFiles/muduo_base.dir/Thread.cc.o.provides

muduo/base/CMakeFiles/muduo_base.dir/Thread.cc.o.provides.build: muduo/base/CMakeFiles/muduo_base.dir/Thread.cc.o


muduo/base/CMakeFiles/muduo_base.dir/ThreadPool.cc.o: muduo/base/CMakeFiles/muduo_base.dir/flags.make
muduo/base/CMakeFiles/muduo_base.dir/ThreadPool.cc.o: ../muduo/base/ThreadPool.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/host_WorkDir/ldd_work/c++_work/tstMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object muduo/base/CMakeFiles/muduo_base.dir/ThreadPool.cc.o"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/muduo_base.dir/ThreadPool.cc.o -c /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/ThreadPool.cc

muduo/base/CMakeFiles/muduo_base.dir/ThreadPool.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/muduo_base.dir/ThreadPool.cc.i"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/ThreadPool.cc > CMakeFiles/muduo_base.dir/ThreadPool.cc.i

muduo/base/CMakeFiles/muduo_base.dir/ThreadPool.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/muduo_base.dir/ThreadPool.cc.s"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/ThreadPool.cc -o CMakeFiles/muduo_base.dir/ThreadPool.cc.s

muduo/base/CMakeFiles/muduo_base.dir/ThreadPool.cc.o.requires:

.PHONY : muduo/base/CMakeFiles/muduo_base.dir/ThreadPool.cc.o.requires

muduo/base/CMakeFiles/muduo_base.dir/ThreadPool.cc.o.provides: muduo/base/CMakeFiles/muduo_base.dir/ThreadPool.cc.o.requires
	$(MAKE) -f muduo/base/CMakeFiles/muduo_base.dir/build.make muduo/base/CMakeFiles/muduo_base.dir/ThreadPool.cc.o.provides.build
.PHONY : muduo/base/CMakeFiles/muduo_base.dir/ThreadPool.cc.o.provides

muduo/base/CMakeFiles/muduo_base.dir/ThreadPool.cc.o.provides.build: muduo/base/CMakeFiles/muduo_base.dir/ThreadPool.cc.o


muduo/base/CMakeFiles/muduo_base.dir/convert.cpp.o: muduo/base/CMakeFiles/muduo_base.dir/flags.make
muduo/base/CMakeFiles/muduo_base.dir/convert.cpp.o: ../muduo/base/convert.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/host_WorkDir/ldd_work/c++_work/tstMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object muduo/base/CMakeFiles/muduo_base.dir/convert.cpp.o"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/muduo_base.dir/convert.cpp.o -c /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/convert.cpp

muduo/base/CMakeFiles/muduo_base.dir/convert.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/muduo_base.dir/convert.cpp.i"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/convert.cpp > CMakeFiles/muduo_base.dir/convert.cpp.i

muduo/base/CMakeFiles/muduo_base.dir/convert.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/muduo_base.dir/convert.cpp.s"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base/convert.cpp -o CMakeFiles/muduo_base.dir/convert.cpp.s

muduo/base/CMakeFiles/muduo_base.dir/convert.cpp.o.requires:

.PHONY : muduo/base/CMakeFiles/muduo_base.dir/convert.cpp.o.requires

muduo/base/CMakeFiles/muduo_base.dir/convert.cpp.o.provides: muduo/base/CMakeFiles/muduo_base.dir/convert.cpp.o.requires
	$(MAKE) -f muduo/base/CMakeFiles/muduo_base.dir/build.make muduo/base/CMakeFiles/muduo_base.dir/convert.cpp.o.provides.build
.PHONY : muduo/base/CMakeFiles/muduo_base.dir/convert.cpp.o.provides

muduo/base/CMakeFiles/muduo_base.dir/convert.cpp.o.provides.build: muduo/base/CMakeFiles/muduo_base.dir/convert.cpp.o


# Object files for target muduo_base
muduo_base_OBJECTS = \
"CMakeFiles/muduo_base.dir/Condition.cc.o" \
"CMakeFiles/muduo_base.dir/CountDownLatch.cc.o" \
"CMakeFiles/muduo_base.dir/Date.cc.o" \
"CMakeFiles/muduo_base.dir/Exception.cc.o" \
"CMakeFiles/muduo_base.dir/Logging.cc.o" \
"CMakeFiles/muduo_base.dir/LogStream.cc.o" \
"CMakeFiles/muduo_base.dir/Timestamp.cc.o" \
"CMakeFiles/muduo_base.dir/TimeZone.cc.o" \
"CMakeFiles/muduo_base.dir/Thread.cc.o" \
"CMakeFiles/muduo_base.dir/ThreadPool.cc.o" \
"CMakeFiles/muduo_base.dir/convert.cpp.o"

# External object files for target muduo_base
muduo_base_EXTERNAL_OBJECTS =

muduo/base/libmuduo_base.a: muduo/base/CMakeFiles/muduo_base.dir/Condition.cc.o
muduo/base/libmuduo_base.a: muduo/base/CMakeFiles/muduo_base.dir/CountDownLatch.cc.o
muduo/base/libmuduo_base.a: muduo/base/CMakeFiles/muduo_base.dir/Date.cc.o
muduo/base/libmuduo_base.a: muduo/base/CMakeFiles/muduo_base.dir/Exception.cc.o
muduo/base/libmuduo_base.a: muduo/base/CMakeFiles/muduo_base.dir/Logging.cc.o
muduo/base/libmuduo_base.a: muduo/base/CMakeFiles/muduo_base.dir/LogStream.cc.o
muduo/base/libmuduo_base.a: muduo/base/CMakeFiles/muduo_base.dir/Timestamp.cc.o
muduo/base/libmuduo_base.a: muduo/base/CMakeFiles/muduo_base.dir/TimeZone.cc.o
muduo/base/libmuduo_base.a: muduo/base/CMakeFiles/muduo_base.dir/Thread.cc.o
muduo/base/libmuduo_base.a: muduo/base/CMakeFiles/muduo_base.dir/ThreadPool.cc.o
muduo/base/libmuduo_base.a: muduo/base/CMakeFiles/muduo_base.dir/convert.cpp.o
muduo/base/libmuduo_base.a: muduo/base/CMakeFiles/muduo_base.dir/build.make
muduo/base/libmuduo_base.a: muduo/base/CMakeFiles/muduo_base.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/host_WorkDir/ldd_work/c++_work/tstMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking CXX static library libmuduo_base.a"
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && $(CMAKE_COMMAND) -P CMakeFiles/muduo_base.dir/cmake_clean_target.cmake
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/muduo_base.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
muduo/base/CMakeFiles/muduo_base.dir/build: muduo/base/libmuduo_base.a

.PHONY : muduo/base/CMakeFiles/muduo_base.dir/build

muduo/base/CMakeFiles/muduo_base.dir/requires: muduo/base/CMakeFiles/muduo_base.dir/Condition.cc.o.requires
muduo/base/CMakeFiles/muduo_base.dir/requires: muduo/base/CMakeFiles/muduo_base.dir/CountDownLatch.cc.o.requires
muduo/base/CMakeFiles/muduo_base.dir/requires: muduo/base/CMakeFiles/muduo_base.dir/Date.cc.o.requires
muduo/base/CMakeFiles/muduo_base.dir/requires: muduo/base/CMakeFiles/muduo_base.dir/Exception.cc.o.requires
muduo/base/CMakeFiles/muduo_base.dir/requires: muduo/base/CMakeFiles/muduo_base.dir/Logging.cc.o.requires
muduo/base/CMakeFiles/muduo_base.dir/requires: muduo/base/CMakeFiles/muduo_base.dir/LogStream.cc.o.requires
muduo/base/CMakeFiles/muduo_base.dir/requires: muduo/base/CMakeFiles/muduo_base.dir/Timestamp.cc.o.requires
muduo/base/CMakeFiles/muduo_base.dir/requires: muduo/base/CMakeFiles/muduo_base.dir/TimeZone.cc.o.requires
muduo/base/CMakeFiles/muduo_base.dir/requires: muduo/base/CMakeFiles/muduo_base.dir/Thread.cc.o.requires
muduo/base/CMakeFiles/muduo_base.dir/requires: muduo/base/CMakeFiles/muduo_base.dir/ThreadPool.cc.o.requires
muduo/base/CMakeFiles/muduo_base.dir/requires: muduo/base/CMakeFiles/muduo_base.dir/convert.cpp.o.requires

.PHONY : muduo/base/CMakeFiles/muduo_base.dir/requires

muduo/base/CMakeFiles/muduo_base.dir/clean:
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base && $(CMAKE_COMMAND) -P CMakeFiles/muduo_base.dir/cmake_clean.cmake
.PHONY : muduo/base/CMakeFiles/muduo_base.dir/clean

muduo/base/CMakeFiles/muduo_base.dir/depend:
	cd /host_WorkDir/ldd_work/c++_work/tstMuduo/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /host_WorkDir/ldd_work/c++_work/tstMuduo /host_WorkDir/ldd_work/c++_work/tstMuduo/muduo/base /host_WorkDir/ldd_work/c++_work/tstMuduo/build /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base /host_WorkDir/ldd_work/c++_work/tstMuduo/build/muduo/base/CMakeFiles/muduo_base.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : muduo/base/CMakeFiles/muduo_base.dir/depend

