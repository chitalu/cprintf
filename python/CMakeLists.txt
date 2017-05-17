cmake_minimum_required(VERSION 2.8)

project(cprintf_capi)

set (output_dir ${CMAKE_CURRENT_BINARY_DIR}/.gfiles/)

execute_process(COMMAND python "${CMAKE_CURRENT_SOURCE_DIR}/gen/cpf_gen.py" "${output_dir}")

add_library(    ${PROJECT_NAME} 
                SHARED 
                "${output_dir}/${PROJECT_NAME}.h"
                "${output_dir}/${PROJECT_NAME}.cpp")

# link to main project
target_link_libraries(${PROJECT_NAME} cprintf)

set_target_properties(  ${PROJECT_NAME} PROPERTIES
                        COMPILE_DEFINITIONS "${CPF_PREPROC_DEFS}"
                        COMPILE_FLAGS "${CPF_BUILD_FLAGS}"
                        INCLUDE_DIRECTORIES "${cprintf_incl_dir};${output_dir}") 

add_custom_command( TARGET ${PROJECT_NAME} PRE_BUILD
                            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                            ${cprintf_path} $<TARGET_FILE_DIR:${PROJECT_NAME}>)
