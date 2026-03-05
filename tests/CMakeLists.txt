add_subdirectory(stack)



find_library(CUNIT_LIBRARIES NAMES cunit)
find_path(CUNIT_INCLUDE_DIRS NAMES CUnit/CUnit.h)


if(CUNIT_LIBRARIES AND CUNIT_INCLUDE_DIRS)

    SET(TEST_SOURCES
        ${STACK_TEST_FILE} 
    )

    create_test_sourcelist (
        DRIVER_SOURCES
        TEST_DRIVER.c 
        ${TEST_SOURCES}
    )

    add_executable (TEST_DRIVER 
        ${DRIVER_SOURCES}
        $<TARGET_OBJECTS:util_logic>
    )

    target_link_libraries(TEST_DRIVER PRIVATE ${CUNIT_LIBRARIES})
    
    # Add the header files to the target
    target_include_directories(TEST_DRIVER PRIVATE 
        ${CUNIT_INCLUDE_DIRS} 
        ${CMAKE_SOURCE_DIR}/inc
    )

    # Add all the ADD_TEST for each test
    foreach (test ${TEST_SOURCES})
        get_filename_component (TName ${test} NAME_WE)
        add_test (NAME ${TName} COMMAND TEST_DRIVER ${TName})
    endforeach ()

else()
    message(WARNING "CUnit not found!")
endif()