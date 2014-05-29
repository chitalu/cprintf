cmake_minimum_required(VERSION 2.8)

#
#   project
#
PROJECT (gtest)

MESSAGE(STATUS "beginning ${PROJECT_NAME} setup...")

MESSAGE(STATUS "mapping library source files")
file(GLOB_RECURSE INCL_FILES "${GTEST_INCLUDE_DIR}/*.h")
#
#	only using two source files from gtest
#
SET( SRC_FILES "${GTEST_EXTRACTION_DIR}/src/gtest_main.cc"
				"${GTEST_EXTRACTION_DIR}/src/gtest-all.cc")

include_directories("${GTEST_INCLUDE_DIR}")
include_directories("${GTEST_EXTRACTION_DIR}")

MESSAGE(STATUS "registering ${PROJECT_NAME} library")

ADD_LIBRARY(${PROJECT_NAME} 
            ${INCL_FILES} 
            ${SRC_FILES})

IF(WIN32)
	SET(GTEST_LIB_FILE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}.lib" PARENT_SCOPE)
ELSEIF(UNIX)
	SET(GTEST_LIB_FILE_PATH "${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}.a" PARENT_SCOPE)
ENDIF()

MESSAGE(STATUS "${PROJECT_NAME} has been built successfully")
