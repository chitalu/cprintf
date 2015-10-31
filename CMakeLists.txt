#
#   What you need from here:
#   
#   cprintf_path - the path to the built project binary (.lib, .dll, .a, or .so)
#   cprintf_output_dir - the directory in which the built binary resides.
#   cprintf_incl_dir - project's include directory
#
#   CPF_BUILD_FLAGS: 
#   [string] variable used to specify more compiler flags than their is defaults
#
#   CPF_BUILD_AS_SHARED_LIB: 
#   [option] that enables project to be built as a shared library file (default 
#   is static)    
#   
#   CPF_BUILD_SAMPLES:
#   [option] to build cprintf sample use-cases (default is OFF)
#
#   CPF_BUILD_TESTS:
#   [option] to build tests projects (also builds a gtest project)
#
#   CPF_CLANG_BUILD (linux only):
#   [option] to build cprintf with clang instead of default GCC
#   you probably want to set the following environment varibles prior to invoking
#   cmake:
#   export CC=/usr/bin/clang
#   export CXX=/usr/bin/clang++
#

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#
#   project
#
PROJECT (cprintf++)

MESSAGE(STATUS "beginning ${PROJECT_NAME} setup...")

SET (cprintf_MAJOR 0)
SET (cprintf_MINOR 1)
SET (cprintf_PATCH 0)

# SET (cprintf_VERSION "${cprintf_MAJOR}.${cprintf_MINOR}.${cprintf_PATCH}")

#
#   default is to build as static library (.lib / .a)
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

#
#   TODO
#
OPTION (CPF_BUILD_C_API "build c api for python bindings project" OFF)

IF (CPF_BUILD_AS_SHARED_LIB)
    SET (CPF_PREPROC_DEFS CPF_BUILD_AS_SHARED CPF_FUNC_EXPORT)
ENDIF()

#
# additional compiler flags
#
SET (CPF_BUILD_FLAGS    "")

#
# set compiler flags (note: strictly no warnings)
#
IF(WIN32)
    SET (CMAKE_CXX_FLAGS    "/WX /EHsc")
    
    LIST (APPEND CPF_PREPROC_DEFS VC_EXTRALEAN WIN32_LEAN_AND_MEAN)
ELSEIF(UNIX)

    IF(CPF_BUILD_AS_SHARED_LIB)
        SET (CPF_BUILD_FLAGS "${CPF_BUILD_FLAGS}")
    ENDIF()

    IF(CPF_BUILD_AS_SHARED_LIB)
        SET (CPF_BUILD_FLAGS "${CPF_BUILD_FLAGS} -fPIC")
    ENDIF()

    SET (CMAKE_C_FLAGS                "-Werror -std=c99")
    SET (CMAKE_C_FLAGS_DEBUG          "-g")
    SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
    SET (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
    SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

    #
    # "-pthread" is necessary for gtest to build
    #
    SET (CMAKE_CXX_FLAGS                "-std=c++1y -Werror -pthread")    
    SET (CMAKE_CXX_FLAGS_DEBUG          "-g")
    SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
    SET (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
    SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

    #
    #   not yet properly setup
    #
    IF(CPF_CLANG_BUILD)
        SET (CMAKE_C_COMPILER   "/usr/bin/clang") 
        SET (CMAKE_CXX_COMPILER "/usr/bin/clang++")

        IF(NOT EXISTS ${CMAKE_CXX_COMPILER})
            MESSAGE(FATAL_ERROR "cprintf build configuration failure: clang not found.")
        ENDIF()
    ENDIF(CPF_CLANG_BUILD)
ENDIF()

#
#   client user variables
#
SET(cprintf_path $<TARGET_FILE:${PROJECT_NAME}> 
    CACHE STRING "${PROJECT_NAME} output path")
SET(cprintf_output_dir $<TARGET_FILE_DIR:${PROJECT_NAME}> 
    CACHE STRING "${PROJECT_NAME} output dir")
SET(cprintf_incl_dir "${CMAKE_CURRENT_SOURCE_DIR}/incl" 
    CACHE STRING "path ${PROJECT_NAME} headers directory")

FILE(GLOB_RECURSE INCL_FILES "${cprintf_incl_dir}/*.hpp" "${cprintf_incl_dir}/*.h")
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

#
#   caching need for use in gtest cmake file
#
SET(CPF_BUILD_FLAGS ${CPF_BUILD_FLAGS} CACHE STRING "adopt same compiler flags.")
SET(CPF_PREPROC_DEFS ${CPF_PREPROC_DEFS} CACHE STRING "adopt same preproc defs.")

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

        SET_TARGET_PROPERTIES(  ${sample_proj_name} PROPERTIES
                                COMPILE_DEFINITIONS "${CPF_PREPROC_DEFS}") 

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
                            INCLUDE_DIRECTORIES "${cpf_test_includes}"
                            COMPILE_DEFINITIONS "${CPF_PREPROC_DEFS}")

    TARGET_LINK_LIBRARIES( cpf_test cprintf gtest)

    IF(CPF_BUILD_AS_SHARED)
        ADD_CUSTOM_COMMAND( TARGET cpf_test PRE_BUILD
                            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                            ${cprintf_path} $<TARGET_FILE_DIR:cpf_test>)
    ENDIF(CPF_BUILD_AS_SHARED)

ENDIF(CPF_BUILD_TESTS)

IF(CPF_BUILD_C_API)
        ADD_SUBDIRECTORY(python)
ENDIF(CPF_BUILD_C_API)

MESSAGE(STATUS "${PROJECT_NAME} setup done!")
