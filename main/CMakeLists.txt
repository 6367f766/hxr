project(main)

file(GLOB_RECURSE SourceFiles LIST_DIRECTORIES false *.h *.cpp)

add_executable(${PROJECT_NAME} ${SourceFiles})

target_link_libraries(${PROJECT_NAME} common_include converters argparse logger colourprint)

