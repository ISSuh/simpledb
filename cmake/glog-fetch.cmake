cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(glog-fetch NONE)

include(ExternalProject)

ExternalProject_Add(glog
  URL https://github.com/google/glog/archive/refs/tags/v0.6.0.tar.gz

  SOURCE_DIR    @GLOG_DOWNLOAD_ROOT@/glog-src
  BINARY_DIR    @GLOG_DOWNLOAD_ROOT@/glog-build

  CMAKE_ARGS 
    -DCMAKE_INSTALL_PREFIX=@GLOG_DOWNLOAD_ROOT@/glog-install
    -DBUILD_SHARED_LIBS=OFF
    -DWITH_GFLAGS=OFF
    -DWITH_GTEST=OFF
    -DWITH_GMOCK=OFF
    -DBUILD_TESTING=OFF
)
