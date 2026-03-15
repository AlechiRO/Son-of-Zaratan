
add_library(stack_logic OBJECT stack.c)
target_include_directories(stack_logic PRIVATE ${CMAKE_SOURCE_DIR}/inc)
