cmake_minimum_required(VERSION 2.8)

#
#   project
#
PROJECT (test_c_printf)

MESSAGE(STATUS "building ${CMAKE_PROJECT_NAME} testing project (${PROJECT_NAME})")

#
# library include directories
#
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../base/incl")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/incl")
include_directories("${GTEST_INCLUDE_DIR}")
include_directories("${GTEST_EXTRACTION_DIR}")

file(GLOB_RECURSE INCL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/incl/*.h")
file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

MESSAGE(STATUS "registering testing build")
ADD_EXECUTABLE( ${PROJECT_NAME} 
                ${INCL_FILES}
                ${SRC_FILES} )

TARGET_LINK_LIBRARIES(	${PROJECT_NAME} 
						${c_printf_LIB_FILE_PATH}
						${GTEST_LIB_FILE_PATH})

MESSAGE(STATUS "${PROJECT_NAME} has been built successfully")
