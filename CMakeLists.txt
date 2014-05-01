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

SET (CMAKE_C_FLAGS "-Wall -std=c99")
SET (CMAKE_CXX_FLAGS "-Wall -std=c++11")

IF(WIN32)

ELSEIF(UNIX)
    
    option(USE_CLANG "build application with clang" ON) # ON is the default
    option(USE_GCC "build application with gcc" OFF) # OFF is the default
    
    IF(USE_CLANG)
        message(STATUS "setting up clang")

        SET (CMAKE_C_COMPILER             "/usr/bin/clang")
        SET (CMAKE_C_FLAGS_DEBUG          "-g")
        SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
        SET (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
        SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

        SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
        SET (CMAKE_CXX_FLAGS_DEBUG          "-g")
        SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
        SET (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
        SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
    ELSE(USE_CLANG)
        #
        # setup GCC 
        #
         
         
        message(STATUS "setting up GCC")
    ENDIF(USE_CLANG)

ELSEIF(APPLE)

ENDIF()

message(STATUS "Adding executable...")

add_executable(c_printf ${c_printf_INCL_FILES} ${c_printf_SRC_FILES})

message(STATUS "Finished project setup!")
