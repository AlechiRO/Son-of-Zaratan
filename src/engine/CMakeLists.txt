add_subdirectory(lexer)
add_subdirectory(printer)
target_include_directories(lexer_logic PRIVATE ${CMAKE_SOURCE_DIR}/inc)
target_include_directories(printer_logic PRIVATE ${CMAKE_SOURCE_DIR}/inc)