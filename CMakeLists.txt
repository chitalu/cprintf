#
#   What you need from here:
#   
#   cprintf_path - the path to the built project binary (.lib, .dll, .a, or .so)
#   cprintf_output_dir - the directory in which the built binary resides.
#   cprintf_incl_dir - project's include directory
#
#   CPF_CONFIG_64 - cmake option to enable building as 64bit, this is for linux only
#                   visual studio users will need to open there IDE as 64bit via -G
#   CPF_STATIC_LIB -
#

cmake_minimum_required(VERSION 2.8)

#
#   project
#
PROJECT (cprintf)

MESSAGE(STATUS "beginning ${PROJECT_NAME} setup...")

SET (cprintf_MAJOR 1)
SET (cprintf_MINOR 0)
SET (cprintf_PATCH 0)

SET (cprintf_VERSION_STRING "${cprintf_MAJOR}.${cprintf_MINOR}.${cprintf_PATCH}")

#
#   default is to build as shared library (.dll / .so)
#
OPTION (CPF_STATIC_LIB "build as static library" OFF)

IF (CPF_STATIC_LIB)
    SET (CPF_PREPROC_DEFS "CPF_BUILD_AS_STATIC")
ELSE(CPF_STATIC_LIB)
     SET (CPF_PREPROC_DEFS "CPF_FUNC_EXPORT")
ENDIF()

#
# set compiler flags
#
IF(WIN32)
    SET (CPF_BUILD_FLAGS "/WX")
    LIST (APPEND CPF_PREPROC_DEFS VC_EXTRALEAN WIN32_LEAN_AND_MEAN)
ELSEIF(UNIX)
    SET (CPF_BUILD_FLAGS "-std=gnu++11 -Werror")
ENDIF()

OPTION (CPF_CONFIG_64 "build 64 bit binary" OFF)

IF(CPF_CONFIG_64)
    IF(UNIX)
        SET(CPF_BUILD_FLAGS "${CPF_BUILD_FLAGS} -m64")
    ENDIF()
ENDIF()

#
#   debug/ release specific settings
#
IF(${CMAKE_BUILD_TYPE} MATCHES Release)
    REMOVE_DEFINITIONS(-D_DEBUG)
ELSE(${CMAKE_BUILD_TYPE} MATCHES Debug)
    IF(UNIX)
        ADD_DEFINITIONS(-D_DEBUG)
    ENDIF(UNIX)
ENDIF()

#
#   user variables
#
SET(cprintf_path "$<TARGET_FILE:${PROJECT_NAME}>" CACHE STRING "${PROJECT_NAME} output path")
SET(cprintf_output_dir "$<TARGET_FILE_DIR:${PROJECT_NAME}>" CACHE STRING "${PROJECT_NAME} output dir")
SET(cprintf_incl_dir "${CMAKE_CURRENT_SOURCE_DIR}/incl" CACHE STRING "path ${PROJECT_NAME} headers directory")

FILE(GLOB_RECURSE INCL_FILES "${cprintf_incl_dir}/*.h")
FILE(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

INCLUDE_DIRECTORIES(${cprintf_incl_dir})
INCLUDE_DIRECTORIES("incl/${PROJECT_NAME}/internal")

#
#   add library
#
IF(CPF_STATIC_LIB)
    SET(CPF_LIB_TYPE STATIC)
ELSE(CPF_STATIC_LIB)
     SET(CPF_LIB_TYPE SHARED)
ENDIF(CPF_STATIC_LIB)

ADD_LIBRARY(${PROJECT_NAME} ${CPF_LIB_TYPE} ${INCL_FILES} ${SRC_FILES})

SET_TARGET_PROPERTIES(  ${PROJECT_NAME} 
                        PROPERTIES PREFIX ""
                        COMPILE_DEFINITIONS ${CPF_PREPROC_DEFS}
                        COMPILE_FLAGS "${CPF_BUILD_FLAGS}") 

MESSAGE(STATUS "${PROJECT_NAME} setup done!")
