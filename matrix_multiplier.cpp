#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono>
#include <filesystem>
#include <string>
#include <omp.h>

#ifdef _WIN32
#include <windows.h>
#endif

namespace fs = std::filesystem;

std::vector<std::vector<int>> readMatrix(std::string filename) {
    std::ifstream file(filename);
    std::vector<std::vector<int>> matrix;
    std::string line;
    while (getline(file, line)) {
        std::vector<int> row;
        std::stringstream ss(line);
        int num;
        while (ss >> num) {
            row.push_back(num);
        }
        matrix.push_back(row);
    }
    file.close();
    return matrix;
}

void writeMatrix(std::string filename, const std::vector<std::vector<int>>& matrix) {
    std::ofstream file(filename);
    int n = matrix.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            file << matrix[i][j] << " ";
        }
        file << std::endl;
    }
    file.close();
}

std::vector<std::vector<int>> multiplyMatrices(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    int n = A.size();
    std::vector<std::vector<int>> C(n, std::vector<int>(n, 0));

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
    return C;
}

int main() {
    std::string basePath = "C:\\Users\\OrangeFruit\\Desktop\\parallel_programming\\lab2\\";
    std::vector<int> sizes = {200, 400, 800, 1200, 1600, 2000};

    std::vector<int> core_counts = {1, 2, 4};
    std::vector<int> thread_counts = {1, 2, 4, 8};

    for (int cores : core_counts) {
        #ifdef _WIN32
        DWORD_PTR mask = (1ULL << cores) - 1;
        SetProcessAffinityMask(GetCurrentProcess(), mask);
        #endif

        for (int threads : thread_counts) {
            omp_set_num_threads(threads);

            std::cout << "Cores: " << cores << " | Threads: " << threads <<  std::endl;

            for (int n : sizes) {
                std::string folderPath = basePath + "matrix_size_" + std::to_string(n) + "\\";

                std::vector<std::vector<int>> A = readMatrix(folderPath + "matrix_A.txt");
                std::vector<std::vector<int>> B = readMatrix(folderPath + "matrix_B.txt");

                auto start = std::chrono::high_resolution_clock::now();
                std::vector<std::vector<int>> C = multiplyMatrices(A, B);
                auto end = std::chrono::high_resolution_clock::now();

                auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

                writeMatrix(folderPath + "result.txt", C);

                int matrixSize = A.size();
                long long operations = 2LL * matrixSize * matrixSize * matrixSize;

                std::cout << "Time: " << time << " microseconds" << std::endl;
                std::cout << "Volume: " << matrixSize << "x" << matrixSize << " (" << operations << " operations)" << std::endl;
                std::cout << std::endl;
            }
        }
    }
    return 0;
}