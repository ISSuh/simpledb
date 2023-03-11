cmake_minimum_required(VERSION 3.5)
project(simpledb_benchmark)

message("SimpleDB Benchmark")

#-------------------------------------------------------------------------------
# Compile Option
#-------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-g -Wall -fPIC")

#-------------------------------------------------------------------------------
# Example Build
#-------------------------------------------------------------------------------
include_directories(
  ${PROJECT_SDB_INCLUDE_DIR}
)

link_directories(
  ${BUILD_DIR}
)

add_executable(${PROJECT_NAME} benchmark.cc)

target_link_libraries(${PROJECT_NAME}
 PRIVATE
  benchmark
  benchmark_main
  pthread
)

set_target_properties(${PROJECT_NAME}
 PROPERTIES
  LINKER_LANGUAGE CXX 
  LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR_EXAMPLES}/benchmark
)
