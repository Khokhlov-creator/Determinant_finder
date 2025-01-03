#include <vector>
#include <string>

using Matrix = std::vector<std::vector<double>>;

Matrix read_matrix_from_file(const std::string &file_path, int &n);
double single_thread_determinant(const Matrix &mat, int n);
double multi_thread_determinant(const Matrix &mat, int n, int num_threads);