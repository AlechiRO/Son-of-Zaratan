add_subdirectory(util)
add_subdirectory(engine)

add_executable(shell shell.c 
    $<TARGET_OBJECTS:util_logic> 
    $<TARGET_OBJECTS:engine_logic>
)


target_include_directories(shell PRIVATE ../inc)