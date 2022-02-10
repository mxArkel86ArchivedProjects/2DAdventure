cmake_minimum_required (VERSION 3.8)

project(BasicGame)

aux_source_directory(. SUB_SOURCES)

set(SOURCE_FILES
  ${SOURCE_FILES}
  ${SUB_SOURCES}
  PARENT_SCOPE
)
#set(SOURCE_FILES ${SOURCE_FILES} imports.h application.h peripherals.h CollisionUtil.h util.h)