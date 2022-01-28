cmake_minimum_required(VERSION 3.20)
project(GoogleTests)

set(lib ${CMAKE_CURRENT_SOURCE_DIR}/../../GoogleTestsLib)
include_directories(${lib}/include ${lib})

enable_testing()
include_directories(..)
add_executable(Google_Tests_run tests.cpp ../Node.cpp ../AVLTree.cpp ../AVLIterator.cpp)
include_directories(${lib}/googletest
        ${lib}/googletest/include)
target_link_libraries(Google_Tests_run gtest gtest_main)

add_test(NAME Google_Tests_run
        COMMAND Google_Tests_run)

find_program(VALGRIND valgrind)
if (VALGRIND)
    add_custom_target(valgrind
            COMMAND "${VALGRIND}" --tool=memcheck --leak-check=yes
            --show-reachable=yes --num-callers=20 --track-fds=yes $<TARGET_FILE:Google_Tests_run>)
endif()