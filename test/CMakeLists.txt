PROJECT (gtest)

SET( gtest_zip_file_name "gtest-1.7.0.zip")
SET(gtest_zip_file_path "${CMAKE_CURRENT_SOURCE_DIR}/${gtest_zip_file_name}")

#
#   unpack gtest files
#
EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E tar xvf ${gtest_zip_file_path}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                OUTPUT_QUIET)

SET(gtest_root "${CMAKE_CURRENT_BINARY_DIR}/gtest-1.7.0")
SET(gtest_incl_dir "${gtest_root}/include" 
    CACHE STRING "google test include dirs")

#
#   only using two source files from gtest
#
SET( gtest_srcs "${gtest_root}/src/gtest-all.cc")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${gtest_srcs})
SET_TARGET_PROPERTIES(  ${PROJECT_NAME} PROPERTIES
                        INCLUDE_DIRECTORIES "${gtest_incl_dir};${gtest_root}"
                        COMPILE_FLAGS "${CPF_BUILD_FLAGS}"
                        COMPILE_DEFINITIONS "${CPF_PREPROC_DEFS}")