cmake_minimum_required (VERSION 3.8)

project(BasicGame)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES ${SOURCE_FILES} WIN32 entry.cpp)
set(SOURCE_FILES ${SOURCE_FILES} util/AppConfig.cpp util/CollisionUtil.cpp util/ImageUtil.cpp util/X.cpp)
set(SOURCE_FILES ${SOURCE_FILES} application.cpp Peripherals.cpp)
set(SOURCE_FILES ${SOURCE_FILES} gameobject/ColorRect.cpp gameobject/EventBox.cpp gameobject/Collider.cpp gameobject/LevelProp.cpp)
set(SOURCE_FILES ${SOURCE_FILES} gameobject/events/DeathEvent.cpp)

add_executable(BasicGame ${SOURCE_FILES} "util/GameConsole.cpp")