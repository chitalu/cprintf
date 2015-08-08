cmake_minimum_required(VERSION 2.8)

project(pycpf)

INCLUDE_DIRECTORIES(${cprintf_incl_dir} ${CMAKE_CURRENT_SOURCE_DIR}/incl)

add_library(${PROJECT_NAME} SHARED 
        ${CMAKE_CURRENT_SOURCE_DIR}/incl/cpf_bind.h 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpf_bind.cpp)

target_link_libraries(${PROJECT_NAME} cprintf)

SET_TARGET_PROPERTIES(  ${PROJECT_NAME} PROPERTIES
                        COMPILE_DEFINITIONS "${CPF_PREPROC_DEFS}"
                        COMPILE_FLAGS "${CPF_BUILD_FLAGS}") 


ADD_CUSTOM_COMMAND( TARGET ${PROJECT_NAME} PRE_BUILD
                            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                            ${cprintf_path} $<TARGET_FILE_DIR:${PROJECT_NAME}>)
