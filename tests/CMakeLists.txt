add_subdirectory(util)
add_subdirectory(lexer)


find_library(CUNIT_LIBRARIES NAMES cunit)
find_path(CUNIT_INCLUDE_DIRS NAMES CUnit/CUnit.h)


if(CUNIT_LIBRARIES AND CUNIT_INCLUDE_DIRS)

    # Stack test executable
    add_executable (STACK_TEST_DRIVER 
        ${STACK_TEST_SOURCE}
    )
    target_link_libraries(STACK_TEST_DRIVER PRIVATE ${CUNIT_LIBRARIES})
    target_include_directories(STACK_TEST_DRIVER PRIVATE 
        ${CUNIT_INCLUDE_DIRS} 
        ${CMAKE_SOURCE_DIR}/inc/modules
        ${CMAKE_SOURCE_DIR}/inc/generics
    )

    # DLL test executable
    add_executable (DLL_TEST_DRIVER
        ${DLL_TEST_SOURCE}
    )
    target_link_libraries(DLL_TEST_DRIVER PRIVATE ${CUNIT_LIBRARIES})
    target_include_directories(DLL_TEST_DRIVER PRIVATE 
        ${CUNIT_INCLUDE_DIRS} 
        ${CMAKE_SOURCE_DIR}/inc/modules
        ${CMAKE_SOURCE_DIR}/inc/generics
    )

    # Lexer Util test executable
    add_executable(LEXER_UTIL_TEST_DRIVER
        ${LEXER_UTIL_TEST_SOURCE}
        $<TARGET_OBJECTS:lexer_util_logic>
        $<TARGET_OBJECTS:lexer_logic>
    )

    target_link_libraries(LEXER_UTIL_TEST_DRIVER PRIVATE ${CUNIT_LIBRARIES})
    target_include_directories(LEXER_UTIL_TEST_DRIVER PRIVATE 
        ${CUNIT_INCLUDE_DIRS} 
        ${CMAKE_SOURCE_DIR}/inc/modules
        ${CMAKE_SOURCE_DIR}/inc/generics
        ${CMAKE_SOURCE_DIR}/inc/globals
    )

    # Array List test executable

    add_executable(ARRAY_LIST_TEST_DRIVER
        ${ARRAY_LIST_TEST_SOURCE}
    )

    target_link_libraries(ARRAY_LIST_TEST_DRIVER PRIVATE ${CUNIT_LIBRARIES})
    target_include_directories(ARRAY_LIST_TEST_DRIVER PRIVATE 
        ${CUNIT_INCLUDE_DIRS} 
        ${CMAKE_SOURCE_DIR}/inc/modules
        ${CMAKE_SOURCE_DIR}/inc/generics
    )

    # Token and Literal test executable

    add_executable(TOKEN_TEST_DRIVER
        ${TOKEN_TEST_SOURCE}
        $<TARGET_OBJECTS:token_logic>
    )
    
    target_link_libraries(TOKEN_TEST_DRIVER PRIVATE ${CUNIT_LIBRARIES})
    target_include_directories(TOKEN_TEST_DRIVER PRIVATE 
        ${CUNIT_INCLUDE_DIRS} 
        ${CMAKE_SOURCE_DIR}/inc/modules
        ${CMAKE_SOURCE_DIR}/inc/generics
    )

    # Adding Tests
    add_test(NAME test_stack COMMAND STACK_TEST_DRIVER)
    add_test(NAME test_dll COMMAND DLL_TEST_DRIVER)
    add_test(NAME test_lexer_util COMMAND LEXER_UTIL_TEST_DRIVER)
    add_test(NAME test_array_list COMMAND ARRAY_LIST_TEST_DRIVER)
    add_test(NAME test_token COMMAND TOKEN_TEST_DRIVER)

else()
    message(WARNING "CUnit not found!")
endif()