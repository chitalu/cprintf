cmake_minimum_required(VERSION 2.8)

#
#   If the user specifies -DCMAKE_BUILD_TYPE on the command line, take 
#   their definition and dump it in the cache along with proper documentation, 
#   otherwise set CMAKE_BUILD_TYPE to Debug prior to calling PROJECT()
#
IF(DEFINED CMAKE_BUILD_TYPE)
   SET  (   CMAKE_BUILD_TYPE 
            ${CMAKE_BUILD_TYPE} 
            CACHE STRING 
            "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel."
        )
ELSE()
   SET  (   CMAKE_BUILD_TYPE Debug 
            CACHE STRING 
            "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel."
        )
ENDIF()

#
#   project
#
PROJECT (c_printf)

MESSAGE(STATUS "beginning ${PROJECT_NAME} setup...")

SET (c_printf_VERSION_STRING "1.0.0")
SET (c_printf_VERSION_MAJOR 1)
SET (c_printf_VERSION_MINOR 0)
SET (c_printf_VERSION_PATCH 0)

IF(WIN32)
    #
    # setup visual studio compiler 
    #
    MESSAGE(STATUS "configuring windows compiler")

    #permits for faster builds
    ADD_DEFINITIONS(-DVC_EXTRALEAN -DWIN32_LEAN_AND_MEAN)

ELSEIF(UNIX)
    #
    # setup GCC 
    #
    MESSAGE(STATUS "configuring GNU Compiler Collection")
    
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11") 
ENDIF()

OPTION (CONFIG_64 "build 64 bit binary" OFF)

IF(CONFIG_64)
    MESSAGE(STATUS "configured for 64 bit build")
    SET(BIT_BUILD_CONFIG "64bit")
    IF(UNIX)
        #
        #   configure gcc for 64 bit build
        #
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
    ENDIF()
ELSE()
    MESSAGE(STATUS "configured for 32 bit build")
    SET(BIT_BUILD_CONFIG "32bit")
ENDIF()

SET(DESTINATION_DIR "${CMAKE_CURRENT_BINARY_DIR}/bin/${BIT_BUILD_CONFIG}")

#
#   debug/ release specific settings
#
IF(${CMAKE_BUILD_TYPE} MATCHES Release)

    MESSAGE(STATUS "creating RELEASE build")
    REMOVE_DEFINITIONS(-D_DEBUG)

ELSE(${CMAKE_BUILD_TYPE} MATCHES Debug)

    MESSAGE(STATUS "creating DEBUG build")

    IF(UNIX)
        ADD_DEFINITIONS(-D_DEBUG)
    ENDIF(UNIX)

ENDIF()

#
# setup paths 
#
IF(WIN32)
   
    SET(c_printf_LIB_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
    SET(c_printf_LIB_STATIC "${c_printf_LIB_OUTPUT_DIR}/${PROJECT_NAME}.lib")
    SET(c_printf_LIB_SHARED "${c_printf_LIB_OUTPUT_DIR}/${PROJECT_NAME}.dll")

    ADD_DEFINITIONS(-DVC_EXTRALEAN -DWIN32_LEAN_AND_MEAN)
ELSEIF(UNIX)
    
    SET(c_printf_LIB_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")
    SET(c_printf_LIB_STATIC "${c_printf_LIB_OUTPUT_DIR}/${PROJECT_NAME}.a")
    SET(c_printf_LIB_SHARED "${c_printf_LIB_OUTPUT_DIR}/${PROJECT_NAME}.so")

ENDIF()

MESSAGE(STATUS "mapping library source files")

file(GLOB_RECURSE INCL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/incl/*.h")
file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

INCLUDE_DIRECTORIES("incl")
INCLUDE_DIRECTORIES("incl/internal")
    
MESSAGE(STATUS "registering library builds")

#
#   build both static and shared libraries
#
SET(static_lib_name ${PROJECT_NAME}_static)
SET(shared_lib_name ${PROJECT_NAME})

ADD_LIBRARY(${shared_lib_name} SHARED ${INCL_FILES} ${SRC_FILES})
ADD_LIBRARY(${static_lib_name} STATIC ${INCL_FILES} ${SRC_FILES})

SET_TARGET_PROPERTIES(${static_lib_name}    PROPERTIES 
                                            OUTPUT_NAME ${static_lib_name}
                                            PREFIX "") #remove "lib" prefix on output file name

SET_TARGET_PROPERTIES(${shared_lib_name}    PROPERTIES 
                                            OUTPUT_NAME ${shared_lib_name}
                                            PREFIX "")

EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E make_directory ${DESTINATION_DIR})

INSTALL(TARGETS ${shared_lib_name} ${static_lib_name} DESTINATION ${DESTINATION_DIR})
