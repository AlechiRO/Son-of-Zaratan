add_subdirectory(lexer)
add_subdirectory(printer)
target_include_directories(lexer_logic PRIVATE 
    ${CMAKE_SOURCE_DIR}/inc/modules
    ${CMAKE_SOURCE_DIR}/inc/generics
    ${CMAKE_SOURCE_DIR}/inc/globals
)
target_include_directories(printer_logic PRIVATE 
    ${CMAKE_SOURCE_DIR}/inc/modules
    ${CMAKE_SOURCE_DIR}/inc/generics
)

target_include_directories(token_logic PRIVATE 
    ${CMAKE_SOURCE_DIR}/inc/modules
    ${CMAKE_SOURCE_DIR}/inc/generics
)