project(SimpleDB_test)

message("Build SimpleDB Test")

include(CTest) 

#-------------------------------------------------------------------------------
# Set Configure
#-------------------------------------------------------------------------------
set(GOOGLETEST_INCLUDE_DIR ${GOOGLETEST_DOWNLOAD_INSTALL_DIR}/include)
set(GOOGLETEST_LIB_DIR ${GOOGLETEST_DOWNLOAD_INSTALL_DIR}/lib)

include_directories(
  ${PROJECT_SDB_INCLUDE_DIR}
  ${GOOGLETEST_INCLUDE_DIR}
)

link_directories(
  ${GOOGLETEST_LIB_DIR}
  ${BUILD_DIR}
)

#-------------------------------------------------------------------------------
# Unit Tests
#-------------------------------------------------------------------------------
enable_testing()

add_subdirectory(engine)

