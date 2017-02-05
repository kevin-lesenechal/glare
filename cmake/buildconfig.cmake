string(TIMESTAMP PROJECT_BUILD_TIME "%Y-%m-%d %H:%M:%S" UTC)
execute_process(COMMAND
                "${GIT_EXECUTABLE}" rev-parse --short=8 HEAD
                WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                OUTPUT_VARIABLE PROJECT_BUILD_COMMIT
                RESULT_VARIABLE retcode
                ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE
                )
if(NOT "${retcode}" STREQUAL "0")
  message(FATAL_ERROR "Could not retrieve git hash number (git: '${GIT_EXECUTABLE}').")
endif()

configure_file(
  src/buildconfig.hpp.in
  include/buildconfig.hpp
  @ONLY NEWLINE_STYLE UNIX
)
