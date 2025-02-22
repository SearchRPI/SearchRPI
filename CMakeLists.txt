cmake_minimum_required(VERSION 3.14)

project(SearchRPI VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

enable_testing()

# Add the include directory (avoids needing relative paths)
include_directories(${CMAKE_SOURCE_DIR}/include)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Find all source files in the src directory
file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cc)

# Find all test source files in the tests directory

file(GLOB_RECURSE TEST_FILES ${CMAKE_SOURCE_DIR}/tests/*.cc)

# Create a library from source files
add_library(SearchRPI ${SRC_FILES})

# Create an executable for all tests
add_executable(all_tests ${TEST_FILES})

target_include_directories(
  SearchRPI PUBLIC
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/src
)

target_include_directories(
  all_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/tests/mock
)

# Link the test executable to SearchRPI and GTest
target_link_libraries(
  all_tests PRIVATE
  SearchRPI
  GTest::gtest_main
  GTest::gmock
  GTest::gmock_main
)

# Include GoogleTest and discover all tests
include(GoogleTest)
gtest_discover_tests(all_tests)
