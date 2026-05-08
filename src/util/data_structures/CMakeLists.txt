
add_library(stack_logic OBJECT stack.c)
add_library(dll_logic OBJECT dll.c)
target_include_directories(stack_logic PRIVATE ${CMAKE_SOURCE_DIR}/inc)
target_include_directories(dll_logic PRIVATE ${CMAKE_SOURCE_DIR}/inc)
