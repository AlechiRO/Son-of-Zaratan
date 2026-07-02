add_library(lexer_util_logic OBJECT lexer_util.c)
target_include_directories(lexer_util_logic PRIVATE 
    ${CMAKE_SOURCE_DIR}/inc/modules
    ${CMAKE_SOURCE_DIR}/inc/generics
    ${CMAKE_SOURCE_DIR}/inc/globals
)