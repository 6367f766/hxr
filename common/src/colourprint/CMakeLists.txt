project(colourprint)

file(GLOB SourceFiles *.h *.cpp)
add_library(${PROJECT_NAME} ${SourceFiles})

# Export include files
target_include_directories(${PROJECT_NAME} INTERFACE
    ${PROJECT_SOURCE_DIR}
)

if(BUILD_UNIT_TESTS)
    add_compile_definitions(UNIT_TEST)
    add_executable(${PROJECT_NAME}_unit_test ${SourceFiles})
    target_link_libraries(${PROJECT_NAME}_unit_test 
        libgtest 
        libgtest_main
        libgmock)
    add_test(NAME ${PROJECT_NAME}_unit_test COMMAND ${PROJECT_NAME}_unit_test)
endif()

