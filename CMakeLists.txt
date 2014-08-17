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

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#
#   project
#
PROJECT (cprintf)

MESSAGE(STATUS "beginning ${PROJECT_NAME} setup...")

SET (cprintf_MAJOR 1)
SET (cprintf_MINOR 0)
SET (cprintf_PATCH 0)

SET (cprintf_VERSION_STRING "${cprintf_MAJOR}.${cprintf_MINOR}.${cprintf_PATCH}")

#
#   default is to build as static library (.dll / .so)
#
OPTION (CPF_BUILD_AS_SHARED_LIB "build as shared library" OFF)

#
#   option to build samples, useful if you wish to learn how to use the library's interface
#
OPTION (CPF_BUILD_SAMPLES "build sample projects" OFF)

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
    SET (CPF_BUILD_FLAGS "-std=gnu++11 -Werror")
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
    
    FILE( GLOB SAMPLE_SRC_FILES "sample/*.cpp" )

    FOREACH( sample_src_file ${SAMPLE_SRC_FILES} )

        GET_FILENAME_COMPONENT(sfname ${sample_src_file} NAME_WE)
        SET(sample_exe_name "cpf_sample_${sfname}")
        ADD_EXECUTABLE( ${sample_exe_name} ${sample_src_file} )
        TARGET_LINK_LIBRARIES( ${sample_exe_name} ${PROJECT_NAME} )

        IF(CPF_BUILD_AS_SHARED)
            ADD_CUSTOM_COMMAND( TARGET ${sample_exe_name} PRE_BUILD
                                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                                ${cprintf_path} $<TARGET_FILE_DIR:${sample_exe_name}>)
        ENDIF(CPF_BUILD_AS_SHARED)

    ENDFOREACH( sample_src_file ${APP_SOURCES} )

ENDIF(CPF_BUILD_SAMPLES)

MESSAGE(STATUS "${PROJECT_NAME} setup done!")
