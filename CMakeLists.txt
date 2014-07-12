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
    SET (CPF_BUILD_FLAGS "")
    LIST (APPEND CPF_PREPROC_DEFS VC_EXTRALEAN WIN32_LEAN_AND_MEAN)
ELSEIF(UNIX)
    #
    # setup GCC flags
    #
    SET (CPF_BUILD_FLAGS "-std=gnu++11")
ENDIF()

OPTION (CPF_CONFIG_64 "build 64 bit binary" OFF)

IF(CPF_CONFIG_64)
    MESSAGE(STATUS "configured for 64 bit build")
    SET(BIT_BUILD_CONFIG "64bit")
    IF(UNIX)
        #
        #   append gcc for 64 bit build flag
        #
        SET(CPF_BUILD_FLAGS "${CPF_BUILD_FLAGS} -m64")
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
SET(c_printf_incl_dir "${CMAKE_CURRENT_SOURCE_DIR}/incl" CACHE STRING "path c_printf headers")

IF(WIN32)
   
    SET(c_printf_LIB_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}" CACHE STRING "binary output directory")
    SET(c_printf_LIB_PATH "${c_printf_LIB_OUTPUT_DIR}/${PROJECT_NAME}.lib" CACHE STRING "binary output path")

ELSEIF(UNIX)
    
    SET(c_printf_LIB_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}" CACHE STRING "binary output directory")
    SET(c_printf_LIB_PATH "${c_printf_LIB_OUTPUT_DIR}/${PROJECT_NAME}.a" CACHE STRING "binary output path")

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

MESSAGE(STATUS "creating ${CPF_BUILD_FLAGS} build")

SET_TARGET_PROPERTIES(  ${PROJECT_NAME} 
                        PROPERTIES PREFIX ""
                        COMPILE_DEFINITIONS ${CPF_PREPROC_DEFS}
                        COMPILE_FLAGS "${CPF_BUILD_FLAGS}") 

#EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E make_directory ${DESTINATION_DIR})

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${DESTINATION_DIR})

MESSAGE(STATUS "${PROJECT_NAME} setup done!")