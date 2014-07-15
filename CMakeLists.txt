#
#   Values of interest:
#
#   c_printf_incl_dir - header files dir
#   c_printf_STATIC_LIB_PATH - static library output path
#   c_printf_SHARED_LIB_PATH - shared library output path
#
#   CPF_CONFIG_64 - build as 64bit (linux only)
#
#
#

cmake_minimum_required(VERSION 2.8)

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
#   default is to build as shared library
#
OPTION (CPF_STATIC_LIB "build as static library" OFF)

IF (CPF_STATIC_LIB)
    SET (CPF_PREPROC_DEFS "CPF_BUILD_AS_STATIC")
ELSE(CPF_STATIC_LIB)
     SET (CPF_PREPROC_DEFS "CPF_FUNC_EXPORT")
ENDIF()

IF(WIN32)
    #
    # setup visual studio compiler flags
    #
    SET (CPF_BUILD_FLAGS "/WX")
    LIST (APPEND CPF_PREPROC_DEFS VC_EXTRALEAN WIN32_LEAN_AND_MEAN)
ELSEIF(UNIX)
    #
    # setup GCC flags
    #
    SET (CPF_BUILD_FLAGS "-std=gnu++11 -Werror")
ENDIF()

OPTION (CPF_CONFIG_64 "build 64 bit binary" OFF)

IF(CPF_CONFIG_64)
    #for windows this must be done from command line via -G arg
    IF(UNIX)
        #
        #   append gcc for 64 bit build flag
        #
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
# setup paths 
#
SET(c_printf_incl_dir "${CMAKE_CURRENT_SOURCE_DIR}/incl" 
    CACHE STRING "path c_printf headers")

IF(WIN32)
   
    SET(c_printf_LIB_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}" 
        CACHE STRING "binary output directory")
    SET(c_printf_LIB_PATH_NO_EXT "${c_printf_LIB_OUTPUT_DIR}/${PROJECT_NAME}" )
    SET(c_printf_STATIC_LIB_PATH "${c_printf_LIB_PATH_NO_EXT}.lib" 
        CACHE STRING "static library output path")
    SET(c_printf_SHARED_LIB_PATH "${c_printf_LIB_PATH_NO_EXT}.dll" 
        CACHE STRING "shared library output path")

ELSEIF(UNIX)
    
    SET(c_printf_LIB_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}" 
        CACHE STRING "library output directory")
    SET(c_printf_LIB_PATH_NO_EXT "${c_printf_LIB_OUTPUT_DIR}/${PROJECT_NAME}" )
    SET(c_printf_STATIC_LIB_PATH "${c_printf_LIB_PATH_NO_EXT}.a" 
        CACHE STRING "static library output path")
    SET(c_printf_SHARED_LIB_PATH "${c_printf_LIB_PATH_NO_EXT}.so" 
        CACHE STRING "shared library output path")

ENDIF()

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