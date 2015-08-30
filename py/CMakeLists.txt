cmake_minimum_required(VERSION 2.8)

project(pycpf)

#
# invoke python intepreter to generate the siurce files 
# TODO: add code check error code
#
execute_process(COMMAND python "${CMAKE_CURRENT_SOURCE_DIR}/gen_lib.py" "${CMAKE_CURRENT_BINARY_DIR}")

add_library(    ${PROJECT_NAME} 
                SHARED 
                "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.h"
                "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.cpp")

target_link_libraries(${PROJECT_NAME} cprintf)

set_target_properties(  ${PROJECT_NAME} PROPERTIES
                        COMPILE_DEFINITIONS "${CPF_PREPROC_DEFS}"
                        COMPILE_FLAGS "${CPF_BUILD_FLAGS}"
                        INCLUDE_DIRECTORIES "${cprintf_incl_dir};${CMAKE_CURRENT_BINARY_DIR}") 

add_custom_command( TARGET ${PROJECT_NAME} PRE_BUILD
                            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                            ${cprintf_path} $<TARGET_FILE_DIR:${PROJECT_NAME}>)
