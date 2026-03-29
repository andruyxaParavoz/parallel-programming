# parallel-programming
Лабораторная работа №2  
Отчётный материал  
Дисциплина: Параллельное программирование  
ФИО: Рябчиков Владислав Евгеньевич  
Группа: 6211-100503D  
1. Задание: 
Модифицировать программу из л/р №1 для параллельной работы по технологии OpenMP.  Провести серию экспериментов с разным количеством потоков (1, 2, 4, 8 и т.д.), разными размерами матриц (примерно 200, 400, 800, 1200, 1600, 2000), с разным количеством вычислительных ядер при наличии технической возможности (1, 2, 4, 8 и т.д. ), иначе использовать фиксированное существующее количество вычислительных ядер, например 4.
2. Теория: 
Алгоритмическая сложность O(N³).
Для ускорения вычислений используется параллелизация, при которой вычисления распределяются между несколькими потоками.
В работе используется технология OpenMP.
4. Реализация параллельного алгоритма:
```cpp
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
```
Директива collapse(2) объединяет два внешних цикла и распределяет итерации между потоками.

5. Исходный код:  
matrix_multiplier.cpp:
```cpp
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
```

5. Результаты экспериментов:
   Влияние числа потоков
   Фиксированное количество ядер - 4
Размер матрицы,1 поток (мкс),2 потока (мкс),4 потока (мкс),8 потоков (мкс)
200x200,47 286,24 505,23 340,23 603 
400x400,394 462,201 829,181 132,182 659 
800x800,3 355 083,1 696 092,1 545 459,1 535 519 
1200x1200,11 814 654,5 836 412,7 945 967,7 861 498 
1600x1600,34 931 867,18 276 421,20 473 300,20 812 275 
2000x2000,75 620 522,38 931 261,44 059 834,44 999 624
![Threads](https://github.com/andruyxaParavoz/parallel-programming/blob/lab2/plots/threads.png)
  
  Влияние числа ядер
  Фиксированное количество потоков - 4
Размер матрицы,1 ядро (мкс),2 ядра (мкс),4 ядра (мкс)
200x200,47 790,51 060,23 340 +2
400x400,399 668,364 478,181 132 +2
800x800,3 416 350,3 159 374,1 545 459 +2
1200x1200,13 263 392,15 664 607,7 945 967 +2
1600x1600,34 235 887,42 144 348,20 473 300 +2
2000x2000,82 931 627,89 389 935,44 059 834 +2
![Cores](https://github.com/andruyxaParavoz/parallel-programming/blob/lab2/plots/cores.png)



6. Вывод:
В ходе выполнения лабораторной работы было исследовано влияние параллелизма с помощью библиотеки OpenMP на производительность алгоритма умножения матриц при различных аппаратных и программных конфигурациях.
Эффект ускорения вычислений достигается на матрицах большого размера. С увеличением числа потоков уменьшается время вычислений. Бесконечное увеличение числа потоков не ведет к улучшению производительности алгоритма.
