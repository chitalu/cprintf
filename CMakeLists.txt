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

IF(WIN32)
        #
        # setup visual studio compiler 
        #

        MESSAGE(STATUS "setting up windows compiler")
        
        SET_PROPERTY (GLOBAL PROPERTY CMAKE_CXX_FLAGS "-WX")

ELSEIF(UNIX)
    
    option(USE_CLANG "build application with clang" ON) # ON is the default
    
    IF(USE_CLANG)
        #
        # setup clang 
        #
        MESSAGE (STATUS "setting up clang")

        SET_PROPERTY (GLOBAL PROPERTY CMAKE_C_COMPILER               "/usr/bin/clang")
        SET_PROPERTY (GLOBAL PROPERTY CMAKE_C_FLAGS_DEBUG            "-g")
        SET_PROPERTY (GLOBAL PROPERTY CMAKE_C_FLAGS_MINSIZEREL       "-Os -DNDEBUG")
        SET_PROPERTY (GLOBAL PROPERTY CMAKE_C_FLAGS_RELEASE          "-O4 -DNDEBUG")
        SET_PROPERTY (GLOBAL PROPERTY CMAKE_C_FLAGS_RELWITHDEBINFO   "-O2 -g")

        SET_PROPERTY (GLOBAL PROPERTY CMAKE_CXX_COMPILER             "/usr/bin/clang++")
        SET_PROPERTY (GLOBAL PROPERTY CMAKE_CXX_FLAGS                "-std=c++11 -Wno-error")
        SET_PROPERTY (GLOBAL PROPERTY CMAKE_CXX_FLAGS_DEBUG          "-g")
        SET_PROPERTY (GLOBAL PROPERTY CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
        SET_PROPERTY (GLOBAL PROPERTY CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
        SET_PROPERTY (GLOBAL PROPERTY CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

    ELSE(USE_CLANG)
        #
        # setup GCC 
        #
        MESSAGE(STATUS "setting up GCC")

        SET_PROPERTY (GLOBAL PROPERTY CMAKE_CXX_FLAGS "-Wno-error -std=gnu++11")
         
    ENDIF(USE_CLANG)

ENDIF()

SET_PROPERTY (GLOBAL PROPERTY CMAKE_C_FLAGS "-std=c99")

MESSAGE(STATUS "mapping library source files")

file(GLOB_RECURSE INCL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/base/incl/*.h")
file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/base/src/*.cpp")

#
#   debug/ release specific settings
#
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    MESSAGE(STATUS "creating debug build")
    ADD_DEFINITIONS(-D_DEBUG)
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
     MESSAGE(STATUS "create release build")
ENDIF()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/base/incl")
    
MESSAGE(STATUS "registering library build")

ADD_LIBRARY(${CMAKE_PROJECT_NAME} 
            ${INCL_FILES} 
            ${SRC_FILES})

IF(WIN32)
    SET(LIB_FILE_EXT "lib")
    SET(c_printf_LIB_FILE_PATH 
    "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_PROJECT_NAME}.${LIB_FILE_EXT}")
ELSEIF(UNIX)
    SET(LIB_FILE_EXT "so")
    SET(c_printf_LIB_FILE_PATH 
    "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.${LIB_FILE_EXT}")
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
    WORKING_DIRECTORY ${GTEST_EXTRACTION_DIR}
    OUTPUT_QUIET
)

MESSAGE (STATUS "unpacked google test files to: ${GTEST_DIR}")

ADD_SUBDIRECTORY(gtest)
ADD_SUBDIRECTORY(test_c_printf)

MESSAGE(STATUS "${CMAKE_PROJECT_NAME} has been built successfully")
