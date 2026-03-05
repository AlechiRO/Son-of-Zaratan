
add_library(engine_logic OBJECT parser.c reader.c)
target_include_directories(engine_logic PRIVATE ${CMAKE_SOURCE_DIR}/inc)