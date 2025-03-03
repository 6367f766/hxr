cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Enable/Disable the export compile commands" FORCE)

enable_testing()

find_package(GTest REQUIRED)
add_library(libgtest ALIAS GTest::gtest)
add_library(libgtest_main ALIAS GTest::gtest_main)
add_library(libgmock ALIAS GTest::gmock)
add_library(libgmock_main ALIAS GTest::gmock_main)

add_subdirectory(main)
add_subdirectory(common)
add_subdirectory(thirdparty)
