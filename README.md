# parallel-programming
Лабораторная работа №1
Отчётный материал
Дисциплина: Параллельное программирование
ФИО: Рябчиков Владислав Евгеньевич
Группа: 6211-100503D
1. Задание: 
Написать программу на языке C/C++ для перемножения двух квадратных матриц.
Исходные данные: файл(ы) содержащие значения исходных матриц.
Выходные данные: файл со значениями результирующей матрицы, время выполнения, объем задачи.
Обязательна автоматизированная верификация результатов вычислений с помощью сторонних библиотек или стороннего ПО (например на Matlab/Python).
2. Теория: 
Алгоритмическая сложность O(N³).
3. Ход работы:
Генерация двух квадратных матриц (размер задаёт пользователь) с помощью модуля matrix_generator.py. Использование: python matrix_generator.py.
Вычисление произведения сгенерированных матриц с помощью модуля matrix_multiplier.cpp и получение результата произведения в файл result.txt, объёма и времени работы программы.
Сравнение результатов отработки программы matrix_multiplier.cpp с помощью верификации вычислений с использованием модуля verify.py. Использование: python verify.py.

4. Исходный код:
matrix_multiplier.cpp
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
matrix_generator.py:
import random

n = int(input("Input matrix size: "))

matrix1 = []
matrix2 = []

for i in range(n):
    row1 = []
    row2 = []
    for j in range(n):
        row1.append(random.randint(0, 9))
        row2.append(random.randint(0, 9))
    matrix1.append(row1)
    matrix2.append(row2)

file1 = open("matrix_A.txt", "w")
for row in matrix1:
    for num in row:
        file1.write(str(num) + " ")
    file1.write("\n")
file1.close()

file2 = open("matrix_B.txt", "w")
for row in matrix2:
    for num in row:
        file2.write(str(num) + " ")
    file2.write("\n")
file2.close()
verify.py
import numpy as np

def read_matrix(filename):
    with open(filename, 'r') as f:
        matrix = []
        for line in f:
            row = [int(x) for x in line.split()]
            matrix.append(row)
    return np.array(matrix)

def main():
    path = "C:\\Users\\OrangeFruit\\Desktop\\parallel_programming\\lab1\\"

    A = read_matrix(path + "matrix_A.txt")
    B = read_matrix(path + "matrix_B.txt")
    C = read_matrix(path + "result.txt")

    expected = np.dot(A, B)

    if np.array_equal(C, expected):
        print("VERIFICATION PASSED: Results match")
    else:
        print("VERIFICATION FAILED: Results do not match")
        diff = np.where(C != expected)
        for i, j in zip(diff[0], diff[1]):
            print(f"Mismatch at [{i}][{j}]: {C[i][j]} vs {expected[i][j]}")

if __name__ == "__main__":
    main()


5. Данные отработки модуля верификации:
============================================================
ВЕРИФИКАЦИЯ РЕЗУЛЬТАТОВ ПЕРЕМНОЖЕНИЯ МАТРИЦ
============================================================
РАЗМЕР  200: Верификация пройдена
РАЗМЕР  400: Верификация пройдена
РАЗМЕР  800: Верификация пройдена
РАЗМЕР 1200: Верификация пройдена
РАЗМЕР 1600: Верификация пройдена
РАЗМЕР 2000: Верификация пройдена
============================================================
ИТОГ: Все результаты успешно верифицированы!
============================================================
6. Данные отработки программы для вычисления произведения матриц:
Time: 46041 microseconds
Volume: 200x200 (16000000 operations)

Time: 393719 microseconds
Volume: 400x400 (128000000 operations)

Time: 3558508 microseconds
Volume: 800x800 (1024000000 operations)

Time: 11794371 microseconds
Volume: 1200x1200 (3456000000 operations)

Time: 30652877 microseconds
Volume: 1600x1600 (8192000000 operations)

Time: 83906604 microseconds
Volume: 2000x2000 (16000000000 operations)

7. Таблица результатов перемножения:
============================================================
ТАБЛИЦА РЕЗУЛЬТАТОВ
============================================================
Размер     Время (мкс)     Время (мс)      Операций            
------------------------------------------------------------
200        46041           46.041          16000000            
400        393719          393.719         128000000           
800        3558508         3558.508        1024000000          
1200       11794371        11794.371       3456000000          
1600       30652877        30652.877       8192000000          
2000       83906604        83906.604       16000000000         
============================================================

8. Графики зависимостей:
![Графики зависимостей](https://github.com/andruyxaParavoz/parallel-programming/blob/lab1/matrix_analysis.png)

9. Вывод:
В ходе выполнения лабораторной работы были созданы: модуль для генерации двух квадратных матриц matrix_generator.py, программа вычисления произведения двух квадратных матриц и записи результата в конечный файл result.txt, модуль для верификации вычислений verify.py с помощью библиотеки NumPy. Программа исправно выполняет вычисления и проходит проверку с помощью вышеописанного модуля. Алгоритмическая сложность эксперементально доказана с помощью тестов алгоритма перемножения матриц разного размера.


