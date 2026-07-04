add_subdirectory(util)

# Isolated object file to facilitate testing token.c
add_library(token_logic OBJECT token.c)

add_library(lexer_logic OBJECT lexer.c token.c)

target_include_directories(lexer_util_logic PRIVATE 
    ${CMAKE_SOURCE_DIR}/inc/modules
    ${CMAKE_SOURCE_DIR}/inc/generics
)
