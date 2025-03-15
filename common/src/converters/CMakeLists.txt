project(converters)

file(GLOB_RECURSE SourceFiles LIST_DIRECTORIES false *.h *.cpp)

add_library(${PROJECT_NAME} STATIC ${SourceFiles})
set(Libs common_include colourprint)

target_link_libraries(${PROJECT_NAME} ${Libs})

# Export include files
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

if(BUILD_UNIT_TESTS)
    add_executable(${PROJECT_NAME}_unit_test ${SourceFiles})
    # NOTE: It's important only this target gets the UNIT_TEST flag.
    # otherwise you end-up in library dependency hell
    target_compile_definitions(${PROJECT_NAME}_unit_test PRIVATE UNIT_TEST)
    target_link_libraries(${PROJECT_NAME}_unit_test PRIVATE
        ${PROJECT_NAME}
        ${Libs}
        libgtest 
        libgtest_main
        libgmock
    )
    add_test(NAME ${PROJECT_NAME}_unit_test COMMAND ${PROJECT_NAME}_unit_test)
endif()
