include(CMakeParseArguments)

find_program(GCOVR_PATH gcovr REQUIRED)

function(generate_coverage_report)
    set(multiValueArgs TARGETS TESTS)
    cmake_parse_arguments(COVERAGE "" "" "${multiValueArgs}" ${ARGN})
    set(COVERAGE_REPORT_DIR "${CMAKE_BINARY_DIR}/report")
    set(COVERAGE_REPORT_FILE "${COVERAGE_REPORT_DIR}/index.html")
    message(STATUS "Setting up coverage report for targets: ${COVERAGE_TARGETS} and tests: ${COVERAGE_TESTS}")

    file(MAKE_DIRECTORY "${COVERAGE_REPORT_DIR}")

    add_custom_target(report
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${COVERAGE_REPORT_DIR}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${COVERAGE_REPORT_DIR}"
        COMMAND ${GCOVR_PATH}
            --root "${CMAKE_SOURCE_DIR}"
            --object-directory "${CMAKE_BINARY_DIR}"
            --html-details
            --html-title "Coverage Report"
            --gcov-executable "gcov -p"
            --print-summary
            --keep
            --filter ".*\\.cc"
            --filter ".*\\.inl"
            --exclude ".*_test\\.cc$$"
            --exclude ".*gtest.*"
            --exclude ".*gmock.*"
            -o "${COVERAGE_REPORT_FILE}"
        COMMENT "Generating HTML coverage report"
        VERBATIM
    )
endfunction()
