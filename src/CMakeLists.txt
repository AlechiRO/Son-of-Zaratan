add_subdirectory(util)
add_subdirectory(engine)

add_executable(shell shell.c 
    $<TARGET_OBJECTS:stack_logic> 
    $<TARGET_OBJECTS:lexer_logic>
    $<TARGET_OBJECTS:printer_logic>
    $<TARGET_OBJECTS:lexer_util_logic>
)


target_include_directories(shell PRIVATE ../inc)