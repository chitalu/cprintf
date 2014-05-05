cmake_minimum_required(VERSION 2.8)

IF(WIN32)
        #
        # setup visual studio compiler 
        #

        MESSAGE(STATUS "setting up windows compiler")
        SET (CMAKE_CXX_FLAGS                "-Wall -Wno-error")

ELSEIF(UNIX)
    
    option(USE_CLANG "build application with clang" ON) # ON is the default
    option(USE_GCC "build application with gcc" OFF) # OFF is the default
    
    IF(USE_CLANG)
        #
        # setup clang 
        #
        MESSAGE(STATUS "setting up clang")

        SET (CMAKE_C_COMPILER               "/usr/bin/clang")
        SET (CMAKE_C_FLAGS_DEBUG            "-g")
        SET (CMAKE_C_FLAGS_MINSIZEREL       "-Os -DNDEBUG")
        SET (CMAKE_C_FLAGS_RELEASE          "-O4 -DNDEBUG")
        SET (CMAKE_C_FLAGS_RELWITHDEBINFO   "-O2 -g")

        SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
        SET (CMAKE_CXX_FLAGS                "-std=c++11 -Wall -Wno-error")
        SET (CMAKE_CXX_FLAGS_DEBUG          "-g")
        SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
        SET (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
        SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

    ELSE(USE_CLANG)
        #
        # setup GCC 
        #

        SET (CMAKE_CXX_FLAGS "-Wall -Wno-error -std=gnu++11")
         
        MESSAGE(STATUS "setting up GCC")

    ENDIF(USE_CLANG)

ENDIF()

SET (CMAKE_C_FLAGS "-Wall -Wno-error -std=c99")

#
#   project
#
PROJECT (c_printf)

MESSAGE(STATUS "beginning ${PROJECT_NAME} setup...")

option(BUILD_AS_EXECUTABLE "build as an application " ON) # ON is the default
option(BUILD_AS_LIBRARY "build as a shared library" OFF) # OFF is the default

SET(CMAKE_BUILD_TYPE Debug)

SET (c_printf_VERSION_STRING "0.1.0")
SET (c_printf_VERSION_MAJOR 0)
SET (c_printf_VERSION_MINOR 1)
SET (c_printf_VERSION_PATCH 0)

MESSAGE(STATUS "mapping project source files")

file(GLOB_RECURSE c_printf_INCL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/incl/*.h")
file(GLOB_RECURSE c_printf_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

#
#   debug/ release specific settings
#
IF(CMAKE_BUILD_TYPE MATCHES Debug)

    MESSAGE(STATUS "creating debug build")
    ADD_DEFINITIONS(-DNDEBUG)

ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)

     MESSAGE(STATUS "create release build")

ENDIF()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/incl")

#
#   add preprocessor definition for build output type 
#   and register the corresponding output build type
#
IF(BUILD_AS_EXECUTABLE)

    MESSAGE(STATUS "building project as executable")
    ADD_DEFINITIONS(-DBUILDING_AS_EXECUTABLE)

    ADD_EXECUTABLE( ${PROJECT_NAME} 
                    ${c_printf_INCL_FILES} 
                    ${c_printf_SRC_FILES})

ELSEIF(BUILD_AS_LIBRARY)
    
    MESSAGE(STATUS "building project as shared library")
    ADD_DEFINITIONS(-DBUILDING_AS_LIBRARY)

    ADD_LIBRARY(${PROJECT_NAME} 
                ${c_printf_INCL_FILES} 
                ${c_printf_SRC_FILES})

ENDIF()

MESSAGE(STATUS "finished!")
MESSAGE(STATUS "${PROJECT_NAME} has been built successfully")
