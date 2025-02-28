cmake_minimum_required(VERSION 3.14)

project(SearchRPI VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Add the include directory (avoids needing relative paths)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Find all source files in the src directory
file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cc)

# Find all test source files in the tests directory
file(GLOB_RECURSE TEST_FILES ${CMAKE_SOURCE_DIR}/tests/*.cc)

# Create an object library from source files
add_library(SearchRPI_obj OBJECT ${SRC_FILES})

# Production library (without TEST_MODE)
add_library(SearchRPI $<TARGET_OBJECTS:SearchRPI_obj>)
target_include_directories(SearchRPI PUBLIC
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/src
)

# Test library (with TEST_MODE)
add_library(SearchRPI_test $<TARGET_OBJECTS:SearchRPI_obj>)
target_compile_definitions(SearchRPI_test PRIVATE TEST_MODE)
target_include_directories(SearchRPI_test PUBLIC
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/src
)

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
  SearchRPI_test
  GTest::gtest_main
  GTest::gmock
  GTest::gmock_main
)

# LMDB Integration
find_package(PkgConfig REQUIRED)
pkg_check_modules(LMDB REQUIRED lmdb)
target_include_directories(SearchRPI PUBLIC ${LMDB_INCLUDE_DIRS})
target_include_directories(all_tests PRIVATE ${LMDB_INCLUDE_DIRS})
target_link_libraries(SearchRPI PUBLIC ${LMDB_LIBRARIES})
target_link_libraries(all_tests PRIVATE ${LMDB_LIBRARIES})

# Crow Integration https://crowcpp.org/master/getting_started/setup/linux/
find_package(Crow)
target_link_libraries(SearchRPI PUBLIC Crow::Crow)
target_link_libraries(all_tests PRIVATE Crow::Crow)

# GoogleTest
include(GoogleTest)
gtest_discover_tests(all_tests)
