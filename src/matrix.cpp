#include "matrix.hpp"
#include <cmath>
#include <thread>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

Matrix read_matrix_from_file(const std::string &file_path, int &n) {
    std::ifstream file(file_path);
    if (!file) {
        std::cerr << "Error: Unable to open file.\n";
        std::exit(EXIT_FAILURE);
    }

    file >> n;
    n--;
    if (n <= 0 || n > 1000) {
        std::cerr << "Error: Invalid matrix size. Must be 1 ≤ N ≤ 1000.\n";
        std::exit(EXIT_FAILURE);
    }

    Matrix mat(n, std::vector<double>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            file >> mat[i][j];
        }
    }
    return mat;
}

Matrix copy_matrix(const Matrix &mat) {
    return mat;
}

double single_thread_determinant(const Matrix &input_mat, int n) {
    Matrix mat = copy_matrix(input_mat);
    int swap_count = 0;

    for (int i = 0; i < n; ++i) {
        if (std::fabs(mat[i][i]) < 1e-9) {
            int pivot_row = i;
            double max_val = std::fabs(mat[i][i]);
            for (int r = i + 1; r < n; ++r) {
                double val = std::fabs(mat[r][i]);
                if (val > max_val) {
                    pivot_row = r;
                    max_val = val;
                }
            }
            if (pivot_row != i) {
                std::swap(mat[i], mat[pivot_row]);
                swap_count++;
            }
            if (std::fabs(mat[i][i]) < 1e-9) {
                return 0.0;
            }
        }

        for (int j = i + 1; j < n; ++j) {
            double factor = mat[j][i] / mat[i][i];
            for (int k = i; k < n; ++k) {
                mat[j][k] -= factor * mat[i][k];
            }
        }
    }

    double determinant = 1.0;
    for (int i = 0; i < n; ++i) {
        determinant *= mat[i][i];
    }

    if (swap_count % 2 == 1) {
        determinant = -determinant;
    }
    return determinant;
}

double multi_thread_determinant(const Matrix &input_mat, int n, int num_threads) {
    Matrix mat = copy_matrix(input_mat);
    int swap_count = 0;

    for (int i = 0; i < n; ++i) {
        if (std::fabs(mat[i][i]) < 1e-9) {
            int pivot_row = i;
            double max_val = std::fabs(mat[i][i]);
            for (int r = i + 1; r < n; ++r) {
                double val = std::fabs(mat[r][i]);
                if (val > max_val) {
                    pivot_row = r;
                    max_val = val;
                }
            }
            if (pivot_row != i) {
                std::swap(mat[i], mat[pivot_row]);
                swap_count++;
            }
            if (std::fabs(mat[i][i]) < 1e-9) {
                return 0.0;
            }
        }

        auto eliminate_rows = [&](int row_start, int row_end) {
            for (int row = row_start; row < row_end; ++row) {
                double factor = mat[row][i] / mat[i][i];
                for (int col = i; col < n; ++col) {
                    mat[row][col] -= factor * mat[i][col];
                }
            }
        };

        int rows_to_eliminate = n - (i + 1);
        if (rows_to_eliminate <= 0) {
            continue;
        }

        int chunk_size = (rows_to_eliminate + num_threads - 1) / num_threads;
        std::vector<std::thread> threads;
        threads.reserve(num_threads);

        for (int t = 0; t < num_threads; ++t) {
            int start = i + 1 + t * chunk_size;
            if (start >= n) break;
            int end = std::min(i + 1 + (t + 1) * chunk_size, n);
            threads.emplace_back(eliminate_rows, start, end);
        }

        for (auto &th : threads) {
            th.join();
        }
    }

    double determinant = 1.0;
    for (int i = 0; i < n; ++i) {
        determinant *= mat[i][i];
    }

    if (swap_count % 2 == 1) {
        determinant = -determinant;
    }
    return determinant;
}