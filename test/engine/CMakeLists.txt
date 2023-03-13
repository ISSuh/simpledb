project(simpledb_engine_test)

message("Build SimpleDB engine Test")

#-------------------------------------------------------------------------------
# Build unit test
#-------------------------------------------------------------------------------

file(GLOB sdb_engine_unittests
  ${PROJECT_SOURCE_DIR}/*.cc
)

add_executable(${PROJECT_NAME}
  ${sdb_engine_unittests}
)

target_include_directories(${PROJECT_NAME}
 PRIVATE
  ${PROJECT_SDB_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
 PRIVATE
  gtest
  gtest_main
  gmock
  gmock_main
  pthread
)

add_test(
  NAME
    ${PROJECT_NAME}
  COMMAND
    ${PROJECT_NAME}
  WORKING_DIRECTORY
    ${BUILD_DIR_TESTS}
)
