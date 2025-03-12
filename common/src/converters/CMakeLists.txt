project(converters)

if(CMAKE_CROSSCOMPILING)
    add_compile_definitions(CROSS_COMPILING)
endif()

file(GLOB SourceFiles *.h *.cpp)
add_library(${PROJECT_NAME} STATIC ${SourceFiles})

target_link_libraries(${PROJECT_NAME} common_include colourprint)

# Export include files
target_include_directories(${PROJECT_NAME} INTERFACE
    ${PROJECT_SOURCE_DIR}
)
