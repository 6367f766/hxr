project(common_include)

if(CMAKE_CROSSCOMPILING)
    add_compile_definitions(CROSS_COMPILING)
endif()

add_library(${PROJECT_NAME} INTERFACE)

target_link_libraries(${PROJECT_NAME} INTERFACE logger)

# Export include files
target_include_directories(${PROJECT_NAME} INTERFACE
    ${PROJECT_SOURCE_DIR}
)
