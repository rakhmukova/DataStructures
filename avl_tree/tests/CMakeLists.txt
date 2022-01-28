cmake_minimum_required(VERSION 3.20)
project(GoogleTests)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.12.1
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(Google_Tests_run tests.cpp)
include_directories(../src)

target_link_libraries(Google_Tests_run avl_tree_lib)
target_link_libraries(Google_Tests_run gtest gtest_main)

add_test(NAME Google_Tests_run
        COMMAND Google_Tests_run)

find_program(VALGRIND valgrind)
if (VALGRIND)
    add_custom_target(valgrind
            COMMAND "${VALGRIND}" --tool=memcheck --leak-check=yes
            --show-reachable=yes --num-callers=20 --track-fds=yes $<TARGET_FILE:Google_Tests_run>)
endif()