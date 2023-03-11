cmake_minimum_required(VERSION 3.5)
project(example)

message("SimpleDB Example")

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

add_executable(example example.cc)

set_target_properties(example
 PROPERTIES
  LINKER_LANGUAGE CXX 
  LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR_EXAMPLES}/example
)
