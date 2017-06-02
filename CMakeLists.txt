CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT (cprintf)

option (CPRINTF_BUILD_AS_SHARED_LIB "build as shared library" OFF)

SET(cprintf_include_directory "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING " ${PROJECT_NAME} include")

if (CPRINTF_BUILD_AS_SHARED_LIB)
    set (CPF_PREPROC_DEFS CPF_BUILD_AS_SHARED CPF_FUNC_EXPORT )
    set(library_type SHARED)
else()
     set(library_type STATIC)
endif()

#
# additional compiler flags
#
set (CPF_FLAGS    "")

if(WIN32)

    set (CPF_BUILD_FLAGS    "${CPF_BUILD_FLAGS}  /WX /EHsc")
    LIST (APPEND CPF_PREPROC_DEFS VC_EXTRALEAN WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS)

elseif(UNIX)
	set (CPF_BUILD_FLAGS "${CPF_BUILD_FLAGS} -std=c++11 -Werror")

    if(CPRINTF_BUILD_AS_SHARED_LIB )
        set (CPF_FLAGS "${CPF_FLAGS} -fPIC")
    endif()

	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set (CPF_FLAGS "${CPF_FLAGS} -stdlib=libstdc++")
    endif()

endif()

add_library(${PROJECT_NAME} 
			${library_type} 
			"${cprintf_include_directory}/cprintf/cprintf.h" 
			"${CMAKE_CURRENT_SOURCE_DIR}/cprintf.cpp")

set_target_properties(  ${PROJECT_NAME} PROPERTIES
                        COMPILE_DEFINITIONS "${CPF_PREPROC_DEFS}"
                        COMPILE_FLAGS "${CPF_BUILD_FLAGS}"
						INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}")

set (EXAMPLES_PROJECT_NAME ${PROJECT_NAME}_examples)

add_executable(${EXAMPLES_PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}_examples.cpp")

target_link_libraries(  ${EXAMPLES_PROJECT_NAME} ${PROJECT_NAME} )

set_target_properties(  ${EXAMPLES_PROJECT_NAME} PROPERTIES
                        COMPILE_FLAGS "${CPF_BUILD_FLAGS}"
                        INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}")

add_custom_command( TARGET ${EXAMPLES_PROJECT_NAME} PRE_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    $<TARGET_FILE:${PROJECT_NAME}> $<TARGET_FILE_DIR:${EXAMPLES_PROJECT_NAME}>)