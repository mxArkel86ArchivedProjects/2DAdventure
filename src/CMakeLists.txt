cmake_minimum_required (VERSION 3.8)

project(BasicGame)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES WIN32 entry.cpp gameobject/ColorRect.h gameobject/ColorRect.cpp util/ConfigMgr.cpp util/ConfigMgr.h util/ImageUtil.h util/ImageUtil.cpp application.cpp application.h Peripherals.cpp Peripherals.h util/CollisionUtil.cpp util/CollisionUtil.h util/util.h util/X.h util/X.cpp imports.h gameobject/GameObject.cpp gameobject/GameObject.h gameobject/LevelProp.h gameobject/LevelProp.cpp)

add_executable(BasicGame ${SOURCE_FILES})