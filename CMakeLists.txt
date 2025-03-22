cmake_minimum_required(VERSION 3.14)
project(SearchRPI VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

include(FetchContent)
FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG        asio-1-24-0
)
FetchContent_MakeAvailable(asio)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # For Windows: Prevent overriding the parent project's compiler/linker settings
FetchContent_MakeAvailable(googletest)

# LMDB (system-provided)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LMDB REQUIRED lmdb)

# Gather source and test files
file(GLOB_RECURSE SRC_FILES  ${CMAKE_SOURCE_DIR}/src/*.cc)
file(GLOB_RECURSE TEST_FILES ${CMAKE_SOURCE_DIR}/tests/*.cc)

# Production library: SearchRPI
add_library(SearchRPI STATIC ${SRC_FILES})
target_include_directories(SearchRPI
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${asio_SOURCE_DIR}/asio/include
    ${LMDB_INCLUDE_DIRS}
)
target_link_libraries(SearchRPI
    PUBLIC
    ${LMDB_LIBRARIES}
)

# Test executable: all_tests
add_executable(all_tests ${TEST_FILES})
target_include_directories(all_tests
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/tests/mock
    ${asio_SOURCE_DIR}/asio/include
    ${LMDB_INCLUDE_DIRS}
)
target_link_libraries(all_tests
    PRIVATE
    SearchRPI
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main
)

# --------------------------
# LMDB Integration
# --------------------------
# Ensure pkg-config is available
find_package(PkgConfig REQUIRED)

# Use pkg-config to find LMDB
pkg_check_modules(LMDB REQUIRED lmdb)

# Add LMDB include directories
target_include_directories(SearchRPI PUBLIC ${LMDB_INCLUDE_DIRS})
target_include_directories(all_tests PRIVATE ${LMDB_INCLUDE_DIRS})

# Link LMDB to your targets
target_link_libraries(SearchRPI PUBLIC ${LMDB_LIBRARIES})
target_link_libraries(all_tests PRIVATE ${LMDB_LIBRARIES})
# --------------------------

# Include GoogleTest and discover all tests
include(GoogleTest)
gtest_discover_tests(all_tests)
