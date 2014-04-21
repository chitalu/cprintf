cmake_minimum_required(VERSION 2.8)

project (c_printf)

message(STATUS "Beginning project setup...")

SET(CMAKE_BUILD_TYPE Debug)

SET (c_printf_VERSION_STRING "0.1.0")
SET (c_printf_VERSION_MAJOR 0)
SET (c_printf_VERSION_MINOR 1)
SET (c_printf_VERSION_PATCH 0)

message(STATUS "Adding project source files")

file(GLOB_RECURSE c_printf_INCL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/incl/*.h")
file(GLOB_RECURSE c_printf_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/incl")

message(STATUS "Adding executable...")

add_executable(c_printf ${c_printf_INCL_FILES} ${c_printf_SRC_FILES})

message(STATUS "Finished project setup!")