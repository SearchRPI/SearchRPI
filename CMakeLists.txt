# Minimum CMake version
cmake_minimum_required(VERSION 3.14)

# Project name and version
project(SearchRPI VERSION 1.0)

# Enable testing
# include(CTest)

# Add the main program
# add_executable(my_program src/main.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Find all test source files in the tests directory
file(GLOB TEST_SOURCES "tests/*.cc")
file(GLOB QUERY_PROCESSING_SOURCES "src/*.cpp" "src/*.cc")

# Create an executable for all tests
add_executable(
  all_tests
  ${TEST_SOURCES}
  ${QUERY_PROCESSING_SOURCES}
)

target_include_directories(
  all_tests PRIVATE
  src
)
# Link GTest library
target_link_libraries(
  all_tests
  GTest::gtest_main
)

# Include GoogleTest and discover all tests
include(GoogleTest)
gtest_discover_tests(all_tests)
