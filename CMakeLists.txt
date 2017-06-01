#
#   What you need from here:
#   
#   cprintf_path - the path to the built project binary (.lib, .dll, .a, or .so)
#   cprintf_output_dir - the directory in which the built binary resides.
#   cprintf_incl_dir - project's include directory
#
#   CPF_BUILD_AS_SHARED_LIB: 
#   [option] that enables project to be built as a shared library file (default 
#   is static)    
#   
#   CPF_BUILD_EXAMPLE:
#   [option] to build cprintf sample use-cases (default is OFF)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT (cprintf)

MESSAGE(STATUS "beginning ${PROJECT_NAME} setup...")

#
#   default is to build as static library (.lib / .a)
#
option (CPF_SHARED "build as shared library" OFF)

#
#   option to generate and build the C interface
#
option (CPF_BUILD_C_API "build c api for python bindings project" OFF)

if (CPF_SHARED)
    set (CPF_PREPROC_DEFS CPF_BUILD_AS_SHARED CPF_FUNC_EXPORT )
endif()

#
# additional compiler flags
#
set (CPF_FLAGS    "")

if (NOT ${CPF_SHARED} AND ${CPF_BUILD_C_API})
   message(STATUS "NOTE: building library as shared for C-API.")
   set(CPF_SHARED ON CACHE BOOL "build as shared library")
endif()

IF(WIN32)

    SET (CPF_BUILD_FLAGS    "${CPF_BUILD_FLAGS}  /WX /EHsc")
    LIST (APPEND CPF_PREPROC_DEFS VC_EXTRALEAN WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS)

ELSEIF(UNIX)
	SET (CPF_BUILD_FLAGS "${CPF_BUILD_FLAGS} -std=c++11 -Werror")

    if(CPF_SHARED OR CPF_BUILD_C_API)
        SET (CPF_FLAGS "${CPF_FLAGS} -fPIC")
    ENDIF()

	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set (CPF_FLAGS "${CPF_FLAGS} -stdlib=libstdc++")
    endif()

ENDIF()

#
#   client user variables
#
SET(cprintf_path $<TARGET_FILE:${PROJECT_NAME}> CACHE STRING "${PROJECT_NAME} output path")
SET(cprintf_output_dir $<TARGET_FILE_DIR:${PROJECT_NAME}> CACHE STRING "${PROJECT_NAME} output dir")
SET(cprintf_incl_dir "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}" CACHE STRING "path ${PROJECT_NAME} headers directory")

if(CPF_SHARED)
    set(library_type SHARED)
else()
     set(library_type STATIC)
endif()

ADD_LIBRARY(${PROJECT_NAME} 
			${library_type} 
			"${cprintf_incl_dir}/cprintf.h" 
			"${CMAKE_CURRENT_SOURCE_DIR}/cprintf.cpp")

SET_TARGET_PROPERTIES(  ${PROJECT_NAME} PROPERTIES
                        COMPILE_DEFINITIONS "${CPF_PREPROC_DEFS}"
                        COMPILE_FLAGS "${CPF_BUILD_FLAGS}"
						INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}") 

set(cprintf_path $<TARGET_FILE:${target_name_cpp}> CACHE STRING "${target_name_cpp} output path")

IF(CPF_BUILD_C_API)

	set (PROJECT_NAME_C cprintf_capi)
    set (generated_files_dir ${CMAKE_CURRENT_BINARY_DIR}/generated_files/)
	set (cprintf_capi_incl_dir ${generated_files_dir} CACHE STRING "path ${PROJECT_NAME} headers directory")

	if (NOT EXISTS ${generated_files_dir})
		file(MAKE_DIRECTORY ${generated_files_dir}) 
	endif() 
	
	execute_process(COMMAND python "${CMAKE_CURRENT_SOURCE_DIR}/python/gen/cpf_gen.py" "${generated_files_dir}")
	
	add_library(    ${PROJECT_NAME_C} 
					SHARED 
					${generated_files_dir}/_cprintf.h
					${generated_files_dir}/_cprintf.cpp)

	# link to main project
	target_link_libraries(${PROJECT_NAME_C} ${target_name_cpp})

	set_target_properties(  ${PROJECT_NAME_C} PROPERTIES
							COMPILE_DEFINITIONS "${CPF_PREPROC_DEFS}"
							COMPILE_FLAGS "${CPF_FLAGS}"
							INCLUDE_DIRECTORIES "${cprintf_incl_dir};${generated_files_dir}") 

	add_custom_command( TARGET ${PROJECT_NAME} PRE_BUILD
								COMMAND ${CMAKE_COMMAND} -E copy_if_different
								${cprintf_path} $<TARGET_FILE_DIR:${PROJECT_NAME}>)

	set(cprintf_capi_path $<TARGET_FILE:${PROJECT_NAME_C}> 
		CACHE STRING "${PROJECT_NAME_C} output path")

ENDIF(CPF_BUILD_C_API)

MESSAGE(STATUS "${PROJECT_NAME} setup done!")
