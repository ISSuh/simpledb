macro(fetcing thirdparty_name download_module_path download_root)
  configure_file(
    ${download_module_path}/${thirdparty_name}-fetch.cmake
    ${download_root}/CMakeLists.txt
    @ONLY
  )

  execute_process(
    COMMAND
      "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY
      ${download_root}
  )

  execute_process(
    COMMAND
      "${CMAKE_COMMAND}" --build . -j 24
    WORKING_DIRECTORY
      ${download_root}
  )
endmacro()