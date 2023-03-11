cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(@GOOGLETEST_NAME@-fetch NONE)

include(ExternalProject)

ExternalProject_Add(@GOOGLETEST_NAME@
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.0.tar.gz
  # GIT_TAG v1.12.0

  SOURCE_DIR    @GOOGLETEST_DOWNLOAD_SRC_DIR@
  BINARY_DIR    @GOOGLETEST_DOWNLOAD_BUILD_DIR@

  CMAKE_ARGS 
    -DCMAKE_INSTALL_PREFIX=@GOOGLETEST_DOWNLOAD_INSTALL_DIR@
)
