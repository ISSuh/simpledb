cmake_minimum_required(VERSION 3.5)
project(simpledb)

#-------------------------------------------------------------------------------
# Set Compiler
#-------------------------------------------------------------------------------
option(LINK_DEBUG "Debug Build" ON)
option(LINK_WITH_TEST "build test" ON)

#-------------------------------------------------------------------------------
# Set Compiler
#-------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(LINK_DEBUG)
  set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -fPIC -pg")
else()
  set(CMAKE_CXX_FLAGS "-O2 -Wall -Wextra -fPIC")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # TODO
endif()

#-------------------------------------------------------------------------------
# Set Configure
#-------------------------------------------------------------------------------
set(SDB_LIBRARY_NAME ${PROJECT_NAME})

set(PROJECT_DIR ${PROJECT_SOURCE_DIR})
set(PROJECT_SDB_INCLUDE_DIR ${PROJECT_DIR}/src)
set(PROJECT_SDB_LINK_SRC_DIR ${PROJECT_DIR}/src/sdb)

set(BUILD_DIR ${PROJECT_DIR}/build)
set(BUILD_DIR_SDB ${BUILD_DIR}/sdb)
set(BUILD_DIR_EXAMPLES ${BUILD_DIR_LINK}/example)
set(BUILD_DIR_TESTS ${BUILD_DIR}/test)

#-------------------------------------------------------------------------------
# Build example
#-------------------------------------------------------------------------------
add_subdirectory(example)

#-------------------------------------------------------------------------------
# build unittest
#-------------------------------------------------------------------------------
set(TEST_DIR ${PROJECT_DIR}/test)

include(${PROJECT_DIR}/cmake/googletest.cmake)

fetch_thridparty(
    ${PROJECT_DIR}/test
    ${PROJECT_DIR}/cmake
    ${BUILD_DIR}/googletest
)

add_subdirectory(${TEST_DIR})

#-------------------------------------------------------------------------------
# build benchmark
#-------------------------------------------------------------------------------
set(BENCHMARK_DIR ${PROJECT_DIR}/benchmark)

include(${PROJECT_DIR}/cmake/benchmark.cmake)

fetch_thridparty(
    ${PROJECT_DIR}/benchmark
    ${PROJECT_DIR}/cmake
    ${BUILD_DIR}/benchmark
)

add_subdirectory(${BENCHMARK_DIR})
