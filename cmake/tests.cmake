if (TEST_SOURCES)
  configure_file(cmake/gtest.cmake.in googletest-download/CMakeLists.txt)
  execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
                  RESULT_VARIABLE result
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
  if (result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
  endif ()
  execute_process(COMMAND ${CMAKE_COMMAND} --build .
                  RESULT_VARIABLE result
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
  if (result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
  endif ()

  # Prevent overriding the parent project's compiler/linker
  # settings on Windows
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

  # Add googletest directly to our build. This defines
  # the gtest and gtest_main targets.
  add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                   ${CMAKE_BINARY_DIR}/googletest-build)

  set(APP_SOURCES ${SOURCES})
  list(REMOVE_ITEM APP_SOURCES "src/main.cpp")
  add_executable(${BIN_NAME}_test ${TEST_SOURCES} ${APP_SOURCES})
  target_link_libraries(${BIN_NAME}_test gtest_main ${APP_LIBS})
  enable_testing()
  add_test(NAME ${BIN_NAME}_test
           COMMAND ${BIN_NAME}_test
           WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif ()
