add_subdirectory(stack)



find_library(CUNIT_LIBRARIES NAMES cunit)
find_path(CUNIT_INCLUDE_DIRS NAMES CUnit/CUnit.h)


if(CUNIT_LIBRARIES AND CUNIT_INCLUDE_DIRS)

    SET(ADDITIONAL_SOURCE_FILES
    $<TARGET_OBJECTS:util_logic>
    )
    # Stack test executable
    add_executable (STACK_TEST_DRIVER 
        ${STACK_TEST_SOURCE}
        ${ADDITIONAL_SOURCE_FILES}
    )
    target_link_libraries(STACK_TEST_DRIVER PRIVATE ${CUNIT_LIBRARIES})
    target_include_directories(STACK_TEST_DRIVER PRIVATE 
        ${CUNIT_INCLUDE_DIRS} 
        ${CMAKE_SOURCE_DIR}/inc
    )
    add_test(NAME test_stack COMMAND STACK_TEST_DRIVER)

else()
    message(WARNING "CUnit not found!")
endif()