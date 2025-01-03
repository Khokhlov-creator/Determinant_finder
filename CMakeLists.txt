cmake_minimum_required(VERSION 3.15)

project(MatrixDeterminant LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(core_files
        src/matrix.cpp
        src/matrix.hpp
)

add_executable(MatrixMain
        src/main.cpp
        ${core_files}
)
target_link_libraries(MatrixMain PRIVATE Threads::Threads)

add_executable(MatrixTests
        tests/catch.cpp
        tests/tests.cpp
        ${core_files}
)

target_link_libraries(MatrixTests PRIVATE Threads::Threads)

foreach(target MatrixMain MatrixTests)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wunreachable-code -Wpedantic)
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        target_compile_options(${target} PRIVATE /W4 /utf-8)
    endif()
endforeach()

enable_testing()
add_test(NAME matrix-tests COMMAND MatrixTests)
