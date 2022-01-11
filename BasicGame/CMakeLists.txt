cmake_minimum_required (VERSION 3.8)

project(BasicGame)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES WIN32 entry.cpp application.cpp application.h Peripherals.cpp Peripherals.h CollisionUtil.h CollisionUtil.cpp util.h X.h X.cpp imports.h)

add_executable(BasicGame ${SOURCE_FILES})