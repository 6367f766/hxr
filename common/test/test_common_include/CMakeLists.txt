project(test_common_include)

file(GLOB SourceFiles *.h *.cpp)

add_executable(${PROJECT_NAME} ${SourceFiles})

target_link_libraries(${PROJECT_NAME} 
    libgtest 
    libgtest_main
    libgmock
    common_include
    converters
    logger
    colourprint
)

add_test(
    NAME ${PROJECT_NAME}
    COMMAND ${PROJECT_NAME}
)
