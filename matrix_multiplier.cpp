#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono>

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

void writeMatrix(std::string filename, std::vector<std::vector<int>> matrix) {
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

std::vector<std::vector<int>> multiplyMatrices(std::vector<std::vector<int>> A, std::vector<std::vector<int>> B) {
    int n = A.size();
    std::vector<std::vector<int>> C;

    for (int i = 0; i < n; i++) {
        std::vector<int> row;
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) {
                sum += A[i][k] * B[k][j];
            }
            row.push_back(sum);
        }
        C.push_back(row);
    }

    return C;
}

int main() {
    std::string path = "C:\\Users\\OrangeFruit\\Desktop\\parallel_programming\\lab1\\";

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::vector<int>> A = readMatrix(path + "matrix_A.txt");
    std::vector<std::vector<int>> B = readMatrix(path + "matrix_B.txt");
    std::vector<std::vector<int>> C = multiplyMatrices(A, B);
    writeMatrix(path + "result.txt", C);

    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    int n = A.size();
    long long operations = 2LL * n * n * n;

    std::cout << "Time: " << time << " microseconds" << std::endl;
    std::cout << "Volume: " << n << "x" << n << " (" << operations << " operations)" << std::endl;

    return 0;
}