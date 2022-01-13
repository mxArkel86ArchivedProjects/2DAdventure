cmake_minimum_required (VERSION 3.8)

project(BasicGame)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES WIN32 entry.cpp ConfigMgr.cpp ConfigMgr.h ImageUtil.h ImageUtil.cpp application.cpp application.h Peripherals.cpp Peripherals.h CollisionUtil.cpp CollisionUtil.h util.h X.h X.cpp imports.h GameObject.cpp GameObject.h LevelProp.h LevelProp.cpp)

add_executable(BasicGame ${SOURCE_FILES})