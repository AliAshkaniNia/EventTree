# This collection of utility functions is designed to make life easier, 
# inspired by the invaluable insights and teachings of 
# Professor Dawid Zalewski at the Saxion University of Applied Sciences

# Guard to prevent multiple inclusions
if(DEFINED INSPIRED_UTILS_INCLUDED)
    return()
endif()
set(INSPIRED_UTILS_INCLUDED TRUE)

# Function to get the absolute path of files of a target
function(get_all_target_files target output_var)
    get_target_property(target_sources ${target} SOURCES)
    set(all_files ${target_sources})

    # Get include directories
    get_target_property(include_dirs ${target} INCLUDE_DIRECTORIES)
    if(NOT include_dirs)
        set(include_dirs ${CMAKE_CURRENT_SOURCE_DIR}/include)
    endif()

    # Find headers in all include directories
    foreach(dir ${include_dirs})
        file(GLOB_RECURSE headers
            "${dir}/*.h"
            "${dir}/*.hpp"
            "${dir}/*.hxx"
            "${dir}/*.hh"
        )
        list(APPEND all_files ${headers})
    endforeach()

    # Get additional properties that might contain files
    get_target_property(interface_sources ${target} INTERFACE_SOURCES)
    if(interface_sources)
        list(APPEND all_files ${interface_sources})
    endif()

    list(REMOVE_DUPLICATES all_files)

    set(absolute_files)
    foreach(file ${all_files})
        if(NOT IS_ABSOLUTE ${file})
            get_filename_component(file ${file} ABSOLUTE)
        endif()
        list(APPEND absolute_files ${file})
    endforeach()

    set(${output_var} "${absolute_files}" PARENT_SCOPE)
endfunction()

# Function to add clang-tidy to a target
# Use auto_tidy = TRUE for automatic tidy checks (run on every build)
# or auto_tidy = FALSE to create a custom tidy target to be called manually.
function(add_clang_tidy target auto_tidy)
    # Check if CMAKE_EXPORT_COMPILE_COMMANDS is set
    if(NOT CMAKE_EXPORT_COMPILE_COMMANDS)
        message(FATAL_ERROR "CMAKE_EXPORT_COMPILE_COMMANDS is not set. Please set it to ON for clang-tidy to work properly.")
        return()
    endif()

    find_program(CLANGTIDY NAMES clang-tidy clang-tidy-18 clang-tidy-17 clang-tidy-16 clang-tidy-15 clang-tidy-14 clang-tidy-13 clang-tidy-12 clang-tidy-11 clang-tidy-10)
    if(NOT CLANGTIDY OR CLANGTIDY MATCHES "NOTFOUND")
        message(FATAL_ERROR "clang-tidy was not found. Tidy checks will be skipped.")
        return()
    endif()

    message(STATUS "Found clang-tidy: ${CLANGTIDY}")

    # Get the number of checks enabled by clang-tidy
    execute_process(
        COMMAND ${CLANGTIDY} -list-checks --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy
        COMMAND wc -l
        OUTPUT_VARIABLE CLANG_TIDY_CHECK_COUNT
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    message(STATUS "Number of checks enabled by clang-tidy: ${CLANG_TIDY_CHECK_COUNT}")

    # Get all files associated with the target
    get_all_target_files(${target} target_files)

    set(tidy_command
        ${CLANGTIDY}
        -p ${CMAKE_BINARY_DIR}
        --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy
        ${target_files}
    )

    if(auto_tidy)
        add_custom_target(${target}_tidy ALL
            COMMAND ${tidy_command}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Running clang-tidy on ${target}"
        )
        add_dependencies(${target} ${target}_tidy)
    else()
        add_custom_target(${target}_tidy
            COMMAND ${tidy_command}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Running clang-tidy on ${target}"
        )

        if(TARGET tidy)
            add_dependencies(tidy ${target}_tidy)
        else()
            add_custom_target(tidy DEPENDS ${target}_tidy)
        endif()
    endif()
endfunction()
# Function to add clang-format to a target
# Use auto_format = TRUE for automatic formatting (format on every build)
# or auto_format = FALSE to create a custom format target to be called manually.
function(add_clang_format target auto_format)

    find_program(CLANGFORMAT NAMES clang-format)
    if(NOT CLANGFORMAT OR CLANGFORMAT MATCHES "NOTFOUND")
        message(FATAL_ERROR "clang-format was not found!")
        return()  # Although not necessary after a fatal error, added for clarity
    endif()

    message(STATUS "Found clang-format: ${CLANGFORMAT}")

    get_all_target_files(${target} clangformat_sources)

    if(auto_format)
        add_custom_target(${target}_format ALL
            COMMAND ${CLANGFORMAT} -style=file -i ${clangformat_sources}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Formatting ${target} with ${CLANGFORMAT}..."
        )
    else()
        add_custom_target(${target}_format
            COMMAND ${CLANGFORMAT} -style=file -i ${clangformat_sources}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Formatting ${target} with ${CLANGFORMAT}..."
        )

        if(TARGET format)
            add_dependencies(format ${target}_format)
        else()
            add_custom_target(format DEPENDS ${target}_format)
        endif()
    endif()
endfunction()

function(add_common_compile_options target)
    target_compile_options(${target} PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic -Werror>
        $<$<AND:$<CXX_COMPILER_ID:GNU,Clang,AppleClang>,$<CONFIG:Release>>:-O3>
        $<$<AND:$<CXX_COMPILER_ID:GNU,Clang,AppleClang>,$<CONFIG:Debug>>:-Og -g>
    )
endfunction()

# Function to add code coverage options
function(add_code_coverage_options target)
    if(NOT (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
        message(FATAL_ERROR "Coverage is only supported for GCC and Clang.")
    endif()

    target_compile_options(${target} PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:--coverage -O0 -g>
    )
    target_link_libraries(${target} PRIVATE gcov)
endfunction()

# Function to add address sanitizer options
function(add_asan_options target)
    target_compile_options(${target} PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-fsanitize=address>
    )
    target_link_options(${target} PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-fsanitize=address>
    )
endfunction()

# Function to add undefined behavior sanitizer options
function(add_ubsan_options target)
    target_compile_options(${target} PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-fsanitize=undefined>
    )
    target_link_options(${target} PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-fsanitize=undefined>
    )
endfunction()

# Function to add thread sanitizer options
function(add_tsan_options target)
    target_compile_options(${target} PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-fsanitize=thread>
    )
    target_link_options(${target} PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-fsanitize=thread>
    )
endfunction()

include(FetchContent)
# Common function to set up a library
function(setup_library LIB_NAME GIT_REPO GIT_TAG)
    string(TOLOWER ${LIB_NAME} LIB_NAME_LOWER)
    
    # Try to find the package
    find_package(${LIB_NAME} QUIET)
    
    if(${LIB_NAME}_FOUND)
        message(STATUS "${LIB_NAME} found on the system. Using the installed version.")
        return()
    else()
        message(STATUS "${LIB_NAME} not found on the system. Fetching ${LIB_NAME}...")
    endif()

    # Declare the content to be fetched
    FetchContent_Declare(
        ${LIB_NAME}
        GIT_REPOSITORY ${GIT_REPO}
        GIT_TAG ${GIT_TAG}
        ${ARGN}
    )

    # Make the content available
    FetchContent_MakeAvailable(${LIB_NAME})

    # Get all properties
    FetchContent_GetProperties(${LIB_NAME})

    # List of all variables to export
    set(VARS_TO_EXPORT
        _POPULATED
        _SOURCE_DIR
        _BINARY_DIR
        _FOUND
        _URL
        _URL_HASH
        _URL_MD5
        _DOWNLOAD_DIR
        _DOWNLOADED_FILE
        _SOURCE_SUBDIR
        _BUILD_MANAGED
        _UPDATES_DISCONNECTED
        _UPDATE_DISCONNECTED
        _EXECUTABLE
    )

    # Export all variables to parent scope
    foreach(VAR ${VARS_TO_EXPORT})
        if(DEFINED ${LIB_NAME}${VAR})
            set(${LIB_NAME}${VAR} ${${LIB_NAME}${VAR}} PARENT_SCOPE)
        endif()
    endforeach()
endfunction()

# Function to set up Google Benchmark
function(setup_benchmark)
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)

    setup_library(benchmark 
                  https://github.com/google/benchmark.git 
                  v1.9.0)
endfunction()

# Function to set up GTest
function(setup_gtest)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    setup_library(GTest 
                  https://github.com/google/googletest.git 
                  v1.14.0)

    setup_library(GMock 
                    https://github.com/google/googletest.git 
                    v1.14.0)

    enable_testing()
    include(GoogleTest)
endfunction()

# Function to create a test target 
function(add_gtest test_target test_sources test_includes)
    add_executable(${test_target} ${test_sources})

    target_include_directories(${test_target} PRIVATE ${test_includes})

    target_link_libraries(${test_target} PRIVATE GTest::gtest_main GTest::gmock)

    add_test(NAME ${test_target} COMMAND ${test_target})

    if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.10)
        gtest_discover_tests(${test_target})
    else()
        message(WARNING "CMake version too old to use ctest with gtest.")
    endif()
endfunction()

# Function to generate coverage reports
function(add_coverage_target output_directory)

    if(NOT (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
        message(FATAL_ERROR "Coverage report generation is only supported for GCC and Clang.")
    endif()

    find_program(LCOV lcov)
    find_program(GENHTML genhtml)
    if((NOT LCOV) OR (NOT GENHTML))
        message(FATAL_ERROR "lcov and/or genhtml not found.")
    endif()

    add_custom_target(generate_coverage_report
        COMMAND ${LCOV} --ignore-errors negative -ignore-errors unused --directory . --capture --output-file coverage.info
        COMMAND ${LCOV} --ignore-errors negative -ignore-errors unused --remove coverage.info "/usr/*" --output-file coverage.info
        COMMAND ${LCOV} --ignore-errors negative --ignore-errors unused --remove coverage.info "*/googletest-src/*" --output-file coverage.info
        COMMAND ${GENHTML} coverage.info --output-directory ${output_directory} 
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating coverage report..."
        VERBATIM
    )
    
endfunction()

