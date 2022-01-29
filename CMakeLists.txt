# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project (BasicGame)
set(WIN32 True)

# Include sub-projects.
link_directories("out/build/x64-Debug/src")
include_directories("deps")

add_subdirectory ("headers")
add_subdirectory ("src")

