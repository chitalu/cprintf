#
#   **EXPORTED VARIABLES
#   
#   cprintf_path - path to built target (C++)
#   cprintf_incl_dir - project's include directory (C++)
#
#   cprintf_capi_path - path to built target (C API)
#   cprintf_capi_incl_dir - project's include directory (C API)
#
#   **OPTIONAL CMAKE VARIABLES
#
#   CPF_FLAGS: 
#   [string] variable used to specify more compiler flags than their is defaults
#
#   CPF_SHARED: 
#   [option] that enables project to be built as a shared library file (default 
#   is static)    
#   
#   CPF_SAMPLES:
#   [option] to build cprintf sample use-cases (default is OFF)
#
#   CPF_TESTS:
#   [option] to build tests projects (also builds a gtest project)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#
#   project
#
project (cprintf)

message(STATUS "beginning ${PROJECT_NAME} setup...")

set (cprintf_MAJOR 0)
set (cprintf_MINOR 1)
set (cprintf_PATCH 0)

set (target_name_c cprintf)
set (target_name_cpp cprintf++)

# set (cprintf_VERSION "${cprintf_MAJOR}.${cprintf_MINOR}.${cprintf_PATCH}")

#
#   default is to build as static library (.lib / .a)
#
option (CPF_SHARED "build as shared library" OFF)

#
#   option to build samples, useful if you wish to learn how to use the library's 
#   interface
#
option (CPF_SAMPLES "build sample projects" OFF)

#
#   option to build test project
#
option (CPF_TESTS "build tests" OFF )

#
#   option to generate and build the C interface
#
option (CPF_BUILD_C_API "build c api for python bindings project" OFF)

if (CPF_SHARED)
    set (CPF_PREPROC_DEFS CPF_BUILD_AS_SHARED CPF_FUNC_EXPORT)
endif()

#
# additional compiler flags
#
set (CPF_FLAGS    "")

if (NOT ${CPF_SHARED} AND ${CPF_BUILD_C_API})
   message(STATUS "NOTE: building library as shared for C-API.")
   set(CPF_SHARED ON CACHE BOOL "build as shared library")
endif()

#
# set compiler flags (note: strictly no warnings)
#
if(WIN32)
    set (CMAKE_CXX_FLAGS    "/WX /EHsc")
    list (APPEND CPF_PREPROC_DEFS VC_EXTRALEAN WIN32_LEAN_AND_MEAN)
elseif(UNIX)
    if(CPF_SHARED OR CPF_BUILD_C_API)
        SET (CPF_FLAGS "${CPF_FLAGS} -fPIC")
    endif()

    set (CMAKE_C_FLAGS                "-Werror -std=c99")
    set (CMAKE_C_FLAGS_DEBUG          "-g")
    set (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
    set (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
    set (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

    #
    # "-pthread" is necessary for gtest to build
    #
    set (CMAKE_CXX_FLAGS                "-std=c++11 -Werror -pthread")    
    set (CMAKE_CXX_FLAGS_DEBUG          "-g")
    set (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
    set (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
    set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g") 
endif()

set(cprintf_incl_dir "${CMAKE_CURRENT_SOURCE_DIR}/incl" 
    CACHE STRING "path ${target_name_cpp} headers directory")

set (incl_dir ${cprintf_incl_dir}/cprintf)
set (src_dir "${CMAKE_CURRENT_SOURCE_DIR}/src")

if(CPF_SHARED)
    set(library_type SHARED)
else()
     set(library_type STATIC)
endif()

add_library(${target_name_cpp} 
            ${library_type} 
            # header files 
            ${incl_dir}/internal/cpf_base.h
            ${incl_dir}/internal/cpf_carg.h
            ${incl_dir}/internal/cpf_cdef.h
            ${incl_dir}/internal/cpf_dbgh.h
            ${incl_dir}/internal/cpf_hlpr.h
            ${incl_dir}/internal/cpf_scan.h
            ${incl_dir}/internal/cpf_srch.h
            ${incl_dir}/internal/cpf_type.h
            ${incl_dir}/internal/cpf_tconf.h
            ${incl_dir}/cprintf.hpp
            # source files
            ${src_dir}/cpf_carg.cpp
            ${src_dir}/cpf_cdef.cpp
            ${src_dir}/cpf_scan.cpp
            ${src_dir}/cpf_srch.cpp
            ${src_dir}/cpf_tconf.cpp
            ${src_dir}/cprintf.cpp)

set (incl_dirs "${incl_dir}/..;${incl_dir};${incl_dir}/internal")
set_target_properties(  ${target_name_cpp} PROPERTIES
                        COMPILE_DEFINITIONS "${CPF_PREPROC_DEFS}"
                        COMPILE_FLAGS "${CPF_FLAGS}"
                        INCLUDE_DIRECTORIES ${incl_dirs}) 

set(cprintf_path $<TARGET_FILE:${target_name_cpp}> 
    CACHE STRING "${target_name_cpp} output path")
#
#   caching need for use in gtest cmake file
#
set(CPF_FLAGS ${CPF_FLAGS} CACHE STRING "adopt same compiler flags.")
set(CPF_PREPROC_DEFS ${CPF_PREPROC_DEFS} CACHE STRING "adopt preproc defs.")

if(CPF_BUILD_C_API)
  set (generated_files_dir ${CMAKE_CURRENT_BINARY_DIR}/generated_files/)
  set (cprintf_capi_incl_dir ${generated_files_dir} 
    CACHE STRING "path ${target_name_c} headers directory")

  if (NOT EXISTS ${generated_files_dir})
    file(MAKE_DIRECTORY ${generated_files_dir}) 
  endif() 
  execute_process(COMMAND python "${CMAKE_CURRENT_SOURCE_DIR}/python/gen/cpf_gen.py" "${generated_files_dir}")
  add_library(    ${target_name_c} 
                  SHARED 
                  ${generated_files_dir}/_cprintf.h
                  ${generated_files_dir}/_cprintf.cpp)
  # link to main project
  target_link_libraries(${target_name_c} ${target_name_cpp})
  set_target_properties(  ${target_name_c} PROPERTIES
                          COMPILE_DEFINITIONS "${CPF_PREPROC_DEFS}"
                          COMPILE_FLAGS "${CPF_FLAGS}"
                          INCLUDE_DIRECTORIES "${cprintf_incl_dir};${generated_files_dir}") 
  add_custom_command( TARGET ${PROJECT_NAME} PRE_BUILD
                              COMMAND ${CMAKE_COMMAND} -E copy_if_different
                              ${cprintf_path} $<TARGET_FILE_DIR:${PROJECT_NAME}>)
  set(cprintf_capi_path $<TARGET_FILE:${target_name_c}> 
      CACHE STRING "${target_name_c} output path")
endif()

if(CPF_SAMPLES)
    macro(SUBDIRLIST result curdir)
        file(GLOB children RELATIVE ${curdir} ${curdir}/*)
        set(dirlist "")
        foreach(child ${children})
            if(IS_DIRECTORY ${curdir}/${child})
                list(APPEND dirlist ${child})
            endif()
        endforeach()
        set(${result} ${dirlist})
    endmacro()

    set (samples_dir "${CMAKE_CURRENT_SOURCE_DIR}/smpl")
    SUBDIRLIST(sample_dirs ${samples_dir})
    foreach( sample_dir ${sample_dirs} )
        set(sample_proj_name "cpf_smpl_${sample_dir}")
        add_executable( ${sample_proj_name} "${samples_dir}/${sample_dir}/main.cpp" )
        target_link_libraries( ${sample_proj_name} ${target_name_cpp} )
        set_target_properties(  ${sample_proj_name} PROPERTIES
                                COMPILE_DEFINITIONS "${CPF_PREPROC_DEFS}"
                                INCLUDE_DIRECTORIES "${cprintf_incl_dir}") 
        if(CPF_SHARED)
            add_custom_command( TARGET ${sample_proj_name} PRE_BUILD
                                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                                ${cprintf_path} $<TARGET_FILE_DIR:${sample_proj_name}>)
        endif()
    endforeach( sample_dir ${sample_dirs} )
endif()

if(CPF_TESTS)
    add_subdirectory(test)
    file( GLOB test_proj_files "${CMAKE_CURRENT_SOURCE_DIR}/test/src/*")
    add_executable(cpf_test ${test_proj_files})
    set(cpf_test_includes ${gtest_incl_dir} ${cprintf_incl_dir})
    set_target_properties(  cpf_test PROPERTIES
                            INCLUDE_DIRECTORIES "${cpf_test_includes}"
                            COMPILE_DEFINITIONS "${CPF_PREPROC_DEFS}")
    target_link_libraries( cpf_test ${target_name_cpp} gtest)
    IF(CPF_SHARED)
        add_custom_command( TARGET cpf_test PRE_BUILD
                            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                            ${cprintf_path} $<TARGET_FILE_DIR:cpf_test>)
    endif()
endif()

message(STATUS "${PROJECT_NAME} setup done!")
