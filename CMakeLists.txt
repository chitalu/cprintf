#
#   What need from here:
#   
#   c_printf_path - the path to the built project binary (.lib, .dll, .a, or .so)
#   c_printf_output_dir - the directory in which the built binary resides.
#   c_printf_incl_dir - project's include directory
#
#   CPF_CONFIG_64 - cmake option to enable building as 64bit, this is for linux only
#                   visual studio users will need to open there IDE as 64bit via -G
#

cmake_minimum_required(VERSION 3.0)

#
#   project
#
PROJECT (c_printf)

MESSAGE(STATUS "beginning ${PROJECT_NAME} setup...")

SET (c_printf_VERSION_STRING "1.0.0")
SET (c_printf_VERSION_MAJOR 1)
SET (c_printf_VERSION_MINOR 0)
SET (c_printf_VERSION_PATCH 0)

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
SET(c_printf_path "$<TARGET_FILE:${PROJECT_NAME}>" CACHE STRING "${PROJECT_NAME} output path")
SET(c_printf_output_dir "$<TARGET_FILE_DIR:${PROJECT_NAME}>" CACHE STRING "${PROJECT_NAME} output dir")
SET(c_printf_incl_dir "${CMAKE_CURRENT_SOURCE_DIR}/incl" CACHE STRING "path ${PROJECT_NAME} headers directory")

FILE(GLOB_RECURSE INCL_FILES "${c_printf_incl_dir}/*.h")
FILE(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

INCLUDE_DIRECTORIES(${c_printf_incl_dir})
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