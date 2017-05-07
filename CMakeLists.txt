cmake_minimum_required(VERSION 3.7)
project(gravityarena)
set(EXECUTABLE_NAME gravityarena)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR})
find_package(SFML 2 COMPONENTS system window graphics audio network REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

set(SOURCE_FILES main.cpp game.h spritesheet.cpp spritesheet.h classes.cpp classes.h)
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})