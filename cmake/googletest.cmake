include(${PROJECT_SOURCE_DIR}/cmake/fetching.cmake)

macro(fetch_googletest thirdparty_dir download_module_path download_root)
  set(GOOGLETEST_NAME googletest)
  set(GOOGLETEST_DOWNLOAD_ROOT ${download_root})
  set(GOOGLETEST_DOWNLOAD_SRC_DIR ${GOOGLETEST_DOWNLOAD_ROOT}/${GOOGLETEST_NAME}-src)
  set(GOOGLETEST_DOWNLOAD_BUILD_DIR ${GOOGLETEST_DOWNLOAD_ROOT}/${GOOGLETEST_NAME}-build)
  set(GOOGLETEST_DOWNLOAD_INSTALL_DIR ${GOOGLETEST_DOWNLOAD_ROOT}/${GOOGLETEST_NAME}-install)

  if(NOT EXISTS ${GOOGLETEST_DOWNLOAD_INSTALL_DIR})
    fetcing(
        ${GOOGLETEST_NAME}
        ${download_module_path}
        ${download_root}
    )
  endif()
endmacro()
