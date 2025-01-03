#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cmath>
#include "matrix.hpp"

void print_help() {
    std::cout << "Usage: MatrixDeterminant [--single | --multi] <matrix_file>\n";
    std::cout << "  --single: Use single-threaded determinant calculation\n";
    std::cout << "  --multi: Use multi-threaded determinant calculation\n";
    std::cout << "  <matrix_file>: Path to the file containing the matrix data\n";
    std::cout << "  --help: Show this help message\n";
}
int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_help();
        return 1;
    }

    std::string mode = argv[1];
    std::string file_path = argv[2];

    if (mode == "--help") {
        print_help();
        return 0;
    }

    int n;
    try {
        Matrix mat = read_matrix_from_file(file_path, n);

        if (mode == "--single") {
            auto start = std::chrono::high_resolution_clock::now();
            double determinant = single_thread_determinant(mat, n);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;

            if (std::isnan(determinant) || std::isinf(determinant)) {
                std::cout << "Single-threaded determinant: Undefined (matrix is singular or has invalid values)\n";
                std::cout << "Time taken: " << elapsed.count() << " seconds\n";
            } else {
                std::cout << "Single-threaded determinant: " << determinant << "\n";
                std::cout << "Time taken: " << elapsed.count() << " seconds\n";
            }
        } else if (mode == "--multi") {
            int num_threads = std::thread::hardware_concurrency();
            std::cout << "Using " << num_threads << " threads\n";
            auto start = std::chrono::high_resolution_clock::now();
            double determinant = multi_thread_determinant(mat, n, num_threads);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;

            if (std::isnan(determinant) || std::isinf(determinant)) {
                std::cout << "Multi-threaded determinant: Undefined (matrix is singular or has invalid values)\n";
                std::cout << "Time taken: " << elapsed.count() << " seconds\n";
            } else {
                std::cout << "Multi-threaded determinant: " << determinant << "\n";
                std::cout << "Time taken: " << elapsed.count() << " seconds\n";
            }
        } else {
            std::cerr << "Error: Unknown option '" << mode << "'\n";
            print_help();
            return 1;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}