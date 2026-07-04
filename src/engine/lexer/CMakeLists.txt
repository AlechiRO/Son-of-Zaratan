add_subdirectory(util)

# Isolated object file to facilitate testing token.c
add_library(token_logic OBJECT token.c)

add_library(lexer_logic OBJECT lexer.c token.c lexer_globals.c)

