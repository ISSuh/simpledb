cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(@THIRD_PARTY_NAME@-fetch NONE)

include(ExternalProject)

ExternalProject_Add(@THIRD_PARTY_NAME@
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.0.tar.gz

  SOURCE_DIR    @GOOGLETEST_DOWNLOAD_SRC_DIR@
  BINARY_DIR    @GOOGLETEST_DOWNLOAD_BUILD_DIR@

  CMAKE_ARGS 
    -DCMAKE_INSTALL_PREFIX=@THIRD_PARTY_DOWNLOAD_INSTALL_DIR@
    -DBUILD_SHARED_LIBS=ON
)
