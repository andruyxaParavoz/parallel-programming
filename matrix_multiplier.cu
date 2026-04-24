#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono>
#include <string>
#include <cuda_runtime.h>
#include <iomanip>

#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort=true) {
    if (code != cudaSuccess) {
        fprintf(stderr, "GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
        if (abort) exit(code);
    }
}

__global__ void matrixMultiplyKernel(float* d_A, float* d_B, float* d_C, int n) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < n && col < n) {
        float sum = 0.0f;
        for (int k = 0; k < n; k++) {
            sum += d_A[row * n + k] * d_B[k * n + col];
        }
        d_C[row * n + col] = sum;
    }
}

std::vector<float> readMatrix(const std::string& filename, int& n) {
    std::ifstream file(filename);
    std::vector<float> matrix;
    if (!file.is_open()) {
        return matrix;
    }

    std::string line;
    n = 0;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        float value;
        int rowCount = 0;
        while (ss >> value) {
            matrix.push_back(value);
            rowCount++;
        }
        if (n == 0) n = rowCount;
    }
    return matrix;
}

struct BlockConfig {
    int x;
    int y;
};

int main() {
    std::string basePath = "C:\\Users\\OrangeFruit\\Desktop\\parallel_programming\\lab4\\";
    std::vector<int> sizes = {200, 400, 800, 1200, 1600, 2000};

    std::vector<BlockConfig> configs = {
            {4, 4},
            {8, 8},
            {16, 8},
            {8, 16},
            {16, 16},
            {32, 8},
            {8, 32},
            {32, 16},
            {16, 32},
            {32, 32}
    };

    std::cout << std::left << std::setw(10) << "Size"
              << std::setw(15) << "Block"
              << std::setw(15) << "Time (mcs)"
              << "First Element Check" << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    for (int n : sizes) {
        int matrixSizeA, matrixSizeB;
        std::string folder = basePath + "matrix_size_" + std::to_string(n) + "\\";

        std::vector<float> h_A = readMatrix(folder + "matrix_A.txt", matrixSizeA);
        std::vector<float> h_B = readMatrix(folder + "matrix_B.txt", matrixSizeB);

        if (h_A.empty() || h_B.empty()) continue;

        int N = matrixSizeA;
        size_t bytes = N * N * sizeof(float);
        float *d_A, *d_B, *d_C;

        gpuErrchk(cudaMalloc(&d_A, bytes));
        gpuErrchk(cudaMalloc(&d_B, bytes));
        gpuErrchk(cudaMalloc(&d_C, bytes));

        gpuErrchk(cudaMemcpy(d_A, h_A.data(), bytes, cudaMemcpyHostToDevice));
        gpuErrchk(cudaMemcpy(d_B, h_B.data(), bytes, cudaMemcpyHostToDevice));

        for (const auto& config : configs) {
            dim3 threadsPerBlock(config.x, config.y);
            dim3 blocksPerGrid((N + threadsPerBlock.x - 1) / threadsPerBlock.x,
                               (N + threadsPerBlock.y - 1) / threadsPerBlock.y);

            cudaEvent_t start, stop;
            gpuErrchk(cudaEventCreate(&start));
            gpuErrchk(cudaEventCreate(&stop));

            matrixMultiplyKernel<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, N);
            gpuErrchk(cudaDeviceSynchronize());

            gpuErrchk(cudaEventRecord(start));
            matrixMultiplyKernel<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, N);
            gpuErrchk(cudaEventRecord(stop));

            gpuErrchk(cudaEventSynchronize(stop));

            float milliseconds = 0;
            gpuErrchk(cudaEventElapsedTime(&milliseconds, start, stop));
            long long time_mcs = static_cast<long long>(milliseconds * 1000);

            float checkVal = 0;
            gpuErrchk(cudaMemcpy(&checkVal, d_C, sizeof(float), cudaMemcpyDeviceToHost));

            std::cout << std::left << std::setw(10) << n
                      << std::setw(15) << (std::to_string(config.x) + "x" + std::to_string(config.y))
                      << std::setw(15) << time_mcs
                      << "Value: " << checkVal << std::endl;

            gpuErrchk(cudaEventDestroy(start));
            gpuErrchk(cudaEventDestroy(stop));
        }

        cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);
        std::cout << "------------------------------------------------------------" << std::endl;
    }

    return 0;
}