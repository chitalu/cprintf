cmake_minimum_required(VERSION 2.8)

#
#   project
#
PROJECT (c_printf)

MESSAGE(STATUS "beginning ${CMAKE_PROJECT_NAME} setup...")

#set_target_properties(${TARGET} PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS})

SET(CMAKE_BUILD_TYPE Debug)

SET (c_printf_VERSION_STRING "1.0.0")
SET (c_printf_VERSION_MAJOR 1)
SET (c_printf_VERSION_MINOR 0)
SET (c_printf_VERSION_PATCH 0)


MESSAGE(STATUS "mapping library source files")

file(GLOB_RECURSE INCL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/base/incl/*.h")
file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/base/src/*.cpp")

#
#   debug/ release specific settings
#
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    MESSAGE(STATUS "creating debug build")

    IF(UNIX)
        ADD_DEFINITIONS(-D_DEBUG)
    ENDIF(UNIX)
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
    MESSAGE(STATUS "create release build")

    REMOVE_DEFINITIONS(-D_DEBUG)
ENDIF()

SET (BASE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/base/incl")
INCLUDE_DIRECTORIES("${BASE_INCLUDE_DIR}/user")
INCLUDE_DIRECTORIES("${BASE_INCLUDE_DIR}/internal")
    
MESSAGE(STATUS "registering library build")

ADD_LIBRARY(${CMAKE_PROJECT_NAME} 
            ${INCL_FILES} 
            ${SRC_FILES})

IF(WIN32)
    #
    # setup visual studio compiler 
    #

    MESSAGE(STATUS "setting up windows compiler")
    #SET (CMAKE_CXX_FLAGS "-WX")

    SET(LIB_FILE_EXT "lib")
    SET(c_printf_LIB_FILE_PATH 
    "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_PROJECT_NAME}.${LIB_FILE_EXT}")

    ADD_DEFINITIONS(-DVC_EXTRALEAN -DWIN32_LEAN_AND_MEAN)
ELSEIF(UNIX)
    #
    # setup GCC 
    #
    MESSAGE(STATUS "setting up GCC")
    SET (CMAKE_CXX_FLAGS "-std=gnu++11 -pthread") #"pthread" is necessary for gtest to build

    SET(LIB_FILE_EXT "a")
    SET(c_printf_LIB_FILE_PATH 
    "${CMAKE_CURRENT_BINARY_DIR}/lib${CMAKE_PROJECT_NAME}.${LIB_FILE_EXT}")
ENDIF()

MESSAGE (STATUS "unpacking google test files")

SET(GTEST_ZIP_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/gtest-1.7.0.zip")
SET(GTEST_EXTRACTION_DIR "${CMAKE_CURRENT_BINARY_DIR}/gtest-1.7.0")

IF(WIN32)
    SET(GTEST_LIB_DIR "${CMAKE_CURRENT_BINARY_DIR}/gtest/${CMAKE_BUILD_TYPE}")
ELSEIF(UNIX)
    SET(GTEST_LIB_DIR "${CMAKE_CURRENT_BINARY_DIR}")
ENDIF()

SET(GTEST_INCLUDE_DIR "${GTEST_EXTRACTION_DIR}/include") 

execute_process( 
    COMMAND "${CMAKE_COMMAND}" -E tar xvf ${GTEST_ZIP_FILE_PATH}
    OUTPUT_QUIET
)

MESSAGE (STATUS "unpacked google test files to: ${GTEST_EXTRACTION_DIR}")

ADD_SUBDIRECTORY(gtest)
ADD_SUBDIRECTORY(test_c_printf)

MESSAGE(STATUS "${CMAKE_PROJECT_NAME} has been built successfully")
