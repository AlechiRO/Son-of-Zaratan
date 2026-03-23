add_subdirectory(util)



find_library(CUNIT_LIBRARIES NAMES cunit)
find_path(CUNIT_INCLUDE_DIRS NAMES CUnit/CUnit.h)


if(CUNIT_LIBRARIES AND CUNIT_INCLUDE_DIRS)

    # Stack test executable
    add_executable (STACK_TEST_DRIVER 
        ${STACK_TEST_SOURCE}
        $<TARGET_OBJECTS:stack_logic>
    )
    target_link_libraries(STACK_TEST_DRIVER PRIVATE ${CUNIT_LIBRARIES})
    target_include_directories(STACK_TEST_DRIVER PRIVATE 
        ${CUNIT_INCLUDE_DIRS} 
        ${CMAKE_SOURCE_DIR}/inc
    )

    # DLL test executable
    add_executable (DLL_TEST_DRIVER
        ${DLL_TEST_SOURCE}
        $<TARGET_OBJECTS:dll_logic>
    )
    target_link_libraries(DLL_TEST_DRIVER PRIVATE ${CUNIT_LIBRARIES})
    target_include_directories(DLL_TEST_DRIVER PRIVATE 
        ${CUNIT_INCLUDE_DIRS} 
        ${CMAKE_SOURCE_DIR}/inc
    )

    # Lexer Util test executable
    add_executable(LEXER_UTIL_TEST_DRIVER
        ${LEXER_UTIL_SOURCE}
        $<TARGET_OBJECTS:lexer_util_logic>
    )

    target_link_libraries(LEXER_UTIL_TEST_DRIVER PRIVATE ${CUNIT_LIBRARIES})
    target_include_directories(LEXER_UTIL_TEST_DRIVER PRIVATE 
        ${CUNIT_INCLUDE_DIRS} 
        ${CMAKE_SOURCE_DIR}/inc
    )

    # Adding Tests
    add_test(NAME test_stack COMMAND STACK_TEST_DRIVER)
    add_test(NAME test_dll COMMAND DLL_TEST_DRIVER)

else()
    message(WARNING "CUnit not found!")
endif()