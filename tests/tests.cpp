#define CATCH_CONFIG_MAIN

#include <iostream>
#include "catch.hpp"
#include "../src/matrix.hpp"

TEST_CASE("Determinant of a 2x2 matrix - Single Thread", "[single-thread]") {
    Matrix mat = {{3, 8}, {4, 6}};
    REQUIRE(single_thread_determinant(mat, 2) == Catch::Approx(-14.0));
}

TEST_CASE("Determinant of a 2x2 matrix - Multi-Thread", "[multi-thread]") {
    Matrix mat = {{3, 8}, {4, 6}};
    REQUIRE(multi_thread_determinant(mat, 2, 2) == Catch::Approx(-14.0));
}

TEST_CASE("Determinant of a 3x3 matrix - Single Thread", "[single-thread]") {
    Matrix mat = {{6, 1, 1}, {4, -2, 5}, {2, 8, 7}};
    REQUIRE(single_thread_determinant(mat, 3) == Catch::Approx(-306.0));
}

TEST_CASE("Determinant of a 3x3 matrix - Multi-Thread", "[multi-thread]") {
    Matrix mat = {{6, 1, 1}, {4, -2, 5}, {2, 8, 7}};
    REQUIRE(multi_thread_determinant(mat, 3, 3) == Catch::Approx(-306.0));
}

TEST_CASE("Determinant of a singular matrix - Single Thread", "[single-thread]") {
    Matrix mat = {{1, 2}, {2, 4}};
    REQUIRE(single_thread_determinant(mat, 2) == Catch::Approx(0.0));
}

TEST_CASE("Determinant of a singular matrix - Multi-Thread", "[multi-thread]") {
    Matrix mat = {{1, 2}, {2, 4}};
    REQUIRE(multi_thread_determinant(mat, 2, 2) == Catch::Approx(0.0));
}

TEST_CASE("Determinant of a 4x4 matrix - Single Thread", "[single-thread]") {
    Matrix mat = {{1, 0, 2, -1}, {3, 0, 0, 5}, {2, 1, 4, -3}, {1, 0, 5, 0}};
    REQUIRE(single_thread_determinant(mat, 4) == Catch::Approx(30.0));
}

TEST_CASE("Determinant of a 4x4 matrix - Multi-Thread", "[multi-thread]") {
    Matrix mat = {{1, 0, 2, -1}, {3, 0, 0, 5}, {2, 1, 4, -3}, {1, 0, 5, 0}};
    REQUIRE(multi_thread_determinant(mat, 4, 4) == Catch::Approx(30.0));
}

TEST_CASE("Edge case: Determinant of 1x1 matrix", "[single-thread][multi-thread]") {
    Matrix mat = {{42}};
    REQUIRE(single_thread_determinant(mat, 1) == Catch::Approx(42.0));
    REQUIRE(multi_thread_determinant(mat, 1, 1) == Catch::Approx(42.0));
}

TEST_CASE("Edge case: Large matrix", "[performance]") {
    int n = 10;
    Matrix mat(n, std::vector<double>(n, 1));
    for (int i = 0; i < n; ++i) {
        mat[i][i] = i + 1;
    }
    REQUIRE(single_thread_determinant(mat, n) != Catch::Approx(0.0));
    REQUIRE(multi_thread_determinant(mat, n, 4) != Catch::Approx(0.0));
}

TEST_CASE("Large matrix test - Single and Multi-Thread", "[large-matrix]") {
    int n = 1000;
    Matrix mat(n, std::vector<double>(n, 1));
    for (int i = 0; i < n; ++i) {
        mat[i][i] = i + 1;
    }

    SECTION("Single-thread large matrix determinant") {
        auto start_time = std::chrono::high_resolution_clock::now();

        double det_single = single_thread_determinant(mat, n);

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

        std::cout << "Single-thread took " << duration_us << " microseconds.\n";
        REQUIRE(det_single != Catch::Approx(0.0));
    }

    SECTION("Multi-thread large matrix determinant") {
        auto start_time = std::chrono::high_resolution_clock::now();

        double det_multi = multi_thread_determinant(mat, n, 8);

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

        std::cout << "Multi-thread took " << duration_us << " microseconds.\n";

        REQUIRE(det_multi != Catch::Approx(0.0));
    }
}