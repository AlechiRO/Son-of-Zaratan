add_subdirectory(util)



find_library(CUNIT_LIBRARIES NAMES cunit)
find_path(CUNIT_INCLUDE_DIRS NAMES CUnit/CUnit.h)


if(CUNIT_LIBRARIES AND CUNIT_INCLUDE_DIRS)

    # This deletes the generated .gcda and .gcno files.
    # created by gcov in the build dir recursively
    # add_custom_target(scrub
    # COMMAND find ${CMAKE_BINARY_DIR} -name "*.gcda" -delete
    # WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    # )

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
        ${LEXER_UTIL_SOURCE}
        $<TARGET_OBJECTS:lexer_util_logic>
    )

    target_link_libraries(LEXER_UTIL_TEST_DRIVER PRIVATE ${CUNIT_LIBRARIES})
    target_include_directories(LEXER_UTIL_TEST_DRIVER PRIVATE 
        ${CUNIT_INCLUDE_DIRS} 
        ${CMAKE_SOURCE_DIR}/inc/modules
        ${CMAKE_SOURCE_DIR}/inc/generics
    )

    # Array List add_executable

    add_executable(ARRAY_LIST_TEST_DRIVER
        ${ARRAY_LIST_SOURCE}
    )

    target_link_libraries(ARRAY_LIST_TEST_DRIVER PRIVATE ${CUNIT_LIBRARIES})
    target_include_directories(ARRAY_LIST_TEST_DRIVER PRIVATE 
        ${CUNIT_INCLUDE_DIRS} 
        ${CMAKE_SOURCE_DIR}/inc/modules
        ${CMAKE_SOURCE_DIR}/inc/generics
    )

    
    # Clean the build as well as generated .gcda and .gcno files. 
    # for every test executable
    # add_dependencies(ARRAY_LIST_TEST_DRIVER scrub)
    # add_dependencies(LEXER_UTIL_TEST_DRIVER scrub)
    # add_dependencies(DLL_TEST_DRIVER scrub)
    # add_dependencies(STACK_TEST_DRIVER scrub)


    # Adding Tests
    add_test(NAME test_stack COMMAND STACK_TEST_DRIVER)
    add_test(NAME test_dll COMMAND DLL_TEST_DRIVER)
    add_test(NAME test_lexer_util COMMAND LEXER_UTIL_TEST_DRIVER)
    add_test(NAME test_array_list COMMAND ARRAY_LIST_TEST_DRIVER)

else()
    message(WARNING "CUnit not found!")
endif()