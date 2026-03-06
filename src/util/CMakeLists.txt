
add_library(util_logic OBJECT stack.c)
target_include_directories(util_logic PRIVATE ${CMAKE_SOURCE_DIR}/inc)
