#
#   What you need from here:
#   
#   cprintf_path - the path to the built project binary (.lib, .dll, .a, or .so)
#   cprintf_output_dir - the directory in which the built binary resides.
#   cprintf_incl_dir - project's include directory
#
#   CPF_CONFIG_64: 
#   [option] to enable building as 64bit, this is for linux only
#   visual studio users will need to open there IDE as 64bit via -G
#
#   CPF_BUILD_AS_SHARED_LIB: 
#   [option] that enables project to be built as a shared library file (default 
#   is static)    
#   
#   CPF_BUILD_SAMPLES:
#   [option] to build cprintf sample use-cases (default is OFF)
#
#   CPF_CLANG_BUILD (linux only):
#   [option] to build cprintf with clang instead of default GCC
#

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#
#   project
#
PROJECT (cprintf)

MESSAGE(STATUS "beginning ${PROJECT_NAME} setup...")

SET (cprintf_MAJOR 1)
SET (cprintf_MINOR 0)
SET (cprintf_PATCH 0)

SET (cprintf_VERSION "${cprintf_MAJOR}.${cprintf_MINOR}.${cprintf_PATCH}")

#
#   default is to build as static library (.dll / .so)
#
OPTION (CPF_BUILD_AS_SHARED_LIB "build as shared library" OFF)

#
#   option to build samples, useful if you wish to learn how to use the library's 
#   interface
#
OPTION (CPF_BUILD_SAMPLES "build sample projects" OFF)

#
#   option to build test project
#
OPTION (CPF_BUILD_TESTS "build tests" OFF )

#
#   option to use clang instead of default GCC on linux
#
OPTION (CPF_CLANG_BUILD "build with clang compiler" OFF )

IF (CPF_BUILD_AS_SHARED_LIB)
    SET (CPF_PREPROC_DEFS CPF_BUILD_AS_SHARED CPF_FUNC_EXPORT)
ENDIF()

#
# set compiler flags (strictly no warnings)
#
IF(WIN32)
    SET (CPF_BUILD_FLAGS "/WX")
    LIST (APPEND CPF_PREPROC_DEFS VC_EXTRALEAN WIN32_LEAN_AND_MEAN)
ELSEIF(UNIX)
    SET (CPF_BUILD_FLAGS "-std=c++0x -Werror")

    IF(CPF_BUILD_TESTS)
        #"pthread" is necessary for gtest to build
        SET (CPF_BUILD_FLAGS "${CPF_BUILD_FLAGS} -pthread")
    ENDIF(CPF_BUILD_TESTS)

    #
    #   note yet properly setup
    #
    IF(CPF_CLANG_BUILD)
        SET (CMAKE_C_COMPILER             "/usr/bin/clang")
        SET (CMAKE_C_FLAGS                "-Wall -std=c99")
        SET (CMAKE_C_FLAGS_DEBUG          "-g")
        SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
        SET (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
        SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

        SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
        SET (CMAKE_CXX_FLAGS                "-Wall")
        SET (CMAKE_CXX_FLAGS_DEBUG          "-g")
        SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
        SET (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
        SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
    ENDIF(CPF_CLANG_BUILD)
ENDIF()

OPTION (CPF_CONFIG_64 "build 64 bit binary" OFF)

IF(CPF_CONFIG_64)
    IF(UNIX)
        SET(CPF_BUILD_FLAGS "${CPF_BUILD_FLAGS} -m64")
    ENDIF()
ENDIF()

#
#   user variables
#
SET(cprintf_path $<TARGET_FILE:${PROJECT_NAME}> 
    CACHE STRING "${PROJECT_NAME} output path")
SET(cprintf_output_dir $<TARGET_FILE_DIR:${PROJECT_NAME}> 
    CACHE STRING "${PROJECT_NAME} output dir")
SET(cprintf_incl_dir "${CMAKE_CURRENT_SOURCE_DIR}/incl" 
    CACHE STRING "path ${PROJECT_NAME} headers directory")

FILE(GLOB_RECURSE INCL_FILES "${cprintf_incl_dir}/*.h")
FILE(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

INCLUDE_DIRECTORIES(${cprintf_incl_dir})
INCLUDE_DIRECTORIES("incl/${PROJECT_NAME}/internal")

#
#   add library where type is determined by user option
#
IF(CPF_BUILD_AS_SHARED_LIB)
    SET(CPF_LIB_TYPE SHARED)
ELSE(CPF_BUILD_AS_SHARED_LIB)
     SET(CPF_LIB_TYPE STATIC)
ENDIF(CPF_BUILD_AS_SHARED_LIB)

ADD_LIBRARY(${PROJECT_NAME} ${CPF_LIB_TYPE} ${INCL_FILES} ${SRC_FILES})

SET_TARGET_PROPERTIES(  ${PROJECT_NAME} PROPERTIES
                        COMPILE_DEFINITIONS "${CPF_PREPROC_DEFS}"
                        COMPILE_FLAGS "${CPF_BUILD_FLAGS}") 

IF(CPF_BUILD_SAMPLES)

    MACRO(SUBDIRLIST result curdir)
        FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
        SET(dirlist "")
        FOREACH(child ${children})
            IF(IS_DIRECTORY ${curdir}/${child})
                LIST(APPEND dirlist ${child})
            ENDIF()
        ENDFOREACH()
        SET(${result} ${dirlist})
    ENDMACRO()

    SET (samples_dir "${CMAKE_CURRENT_SOURCE_DIR}/smpl")

    SUBDIRLIST(sample_dirs ${samples_dir})

    FOREACH( sample_dir ${sample_dirs} )

        SET(sample_proj_name "cpf_smpl_${sample_dir}")

        ADD_EXECUTABLE( ${sample_proj_name} "${samples_dir}/${sample_dir}/main.cpp" )
        TARGET_LINK_LIBRARIES( ${sample_proj_name} ${PROJECT_NAME} )

        IF(CPF_BUILD_AS_SHARED)
            ADD_CUSTOM_COMMAND( TARGET ${sample_proj_name} PRE_BUILD
                                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                                ${cprintf_path} $<TARGET_FILE_DIR:${sample_proj_name}>)
        ENDIF(CPF_BUILD_AS_SHARED)

    ENDFOREACH( sample_dir ${sample_dirs} )

ENDIF(CPF_BUILD_SAMPLES)

IF(CPF_BUILD_TESTS)

    ADD_SUBDIRECTORY(test)

    FILE( GLOB test_proj_files "${CMAKE_CURRENT_SOURCE_DIR}/test/src/*")
    ADD_EXECUTABLE(cpf_test ${test_proj_files})

    SET(cpf_test_includes ${gtest_incl_dir} ${cprintf_incl_dir})
    SET_TARGET_PROPERTIES(  cpf_test PROPERTIES
                            INCLUDE_DIRECTORIES "${cpf_test_includes}")

    TARGET_LINK_LIBRARIES( cpf_test ${CMAKE_PROJECT_NAME} gtest)

    IF(CPF_BUILD_AS_SHARED)
        ADD_CUSTOM_COMMAND( TARGET cpf_test PRE_BUILD
                            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                            ${cprintf_path} $<TARGET_FILE_DIR:cpf_test>)
    ENDIF(CPF_BUILD_AS_SHARED)

ENDIF(CPF_BUILD_TESTS)

MESSAGE(STATUS "${PROJECT_NAME} setup done!")
