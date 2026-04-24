#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono>
#include <string>
#include <mpi.h>

std::vector<int> readMatrixFlattened(std::string filename, int& n) {
    std::ifstream file(filename);
    std::vector<int> matrix;
    std::string line;
    n = 0;
    while (getline(file, line)) {
        std::stringstream ss(line);
        int num, count = 0;
        while (ss >> num) {
            matrix.push_back(num);
            count++;
        }
        if (n == 0) n = count;
    }
    file.close();
    return matrix;
}

void writeMatrix(std::string filename, const std::vector<int>& matrix, int n) {
    std::ofstream file(filename);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            file << matrix[i * n + j] << " ";
        }
        file << std::endl;
    }
    file.close();
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::string basePath = "C:\\Users\\OrangeFruit\\Desktop\\parallel_programming\\lab3\\";
    std::vector<int> sizes = {200, 400, 800, 1200, 1600, 2000};

    for (int n : sizes) {
        int rows_per_proc = n / size;
        std::vector<int> A_flat, B_flat, C_flat;
        std::vector<int> local_A(rows_per_proc * n);
        std::vector<int> local_C(rows_per_proc * n);

        if (rank == 0) {
            std::string folderPath = basePath + "matrix_size_" + std::to_string(n) + "\\";
            int temp_n;
            A_flat = readMatrixFlattened(folderPath + "matrix_A.txt", temp_n);
            B_flat = readMatrixFlattened(folderPath + "matrix_B.txt", temp_n);
            C_flat.resize(n * n);
        }

        if (rank != 0) B_flat.resize(n * n);
        MPI_Bcast(B_flat.data(), n * n, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(A_flat.data(), rows_per_proc * n, MPI_INT, local_A.data(), rows_per_proc * n, MPI_INT, 0, MPI_COMM_WORLD);

        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < rows_per_proc; i++) {
            for (int j = 0; j < n; j++) {
                int sum = 0;
                for (int k = 0; k < n; k++) {
                    sum += local_A[i * n + k] * B_flat[k * n + j];
                }
                local_C[i * n + j] = sum;
            }
        }

        MPI_Gather(local_C.data(), rows_per_proc * n, MPI_INT, C_flat.data(), rows_per_proc * n, MPI_INT, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            auto end = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            std::cout << size << "," << n << "," << time << std::endl;
        }
    }
    MPI_Finalize();
    return 0;
}