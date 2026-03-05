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
matrix_multiplier.cpp:
```cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono>
#include <filesystem>
#include <string>

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
    std::string basePath = "C:\\Users\\OrangeFruit\\Desktop\\parallel_programming\\lab1\\";

    std::vector<int> sizes = {200, 400, 800, 1200, 1600, 2000};

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
    return 0;
}
```

matrix_generator.py:
```python
import random
import os

n = int(input("Input matrix size: "))
folder_name = f"matrix_size_{n}"
os.makedirs(folder_name, exist_ok=True)

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

file1 = open(os.path.join(folder_name, "matrix_A.txt"), "w")
for row in matrix1:
    for num in row:
        file1.write(str(num) + " ")
    file1.write("\n")
file1.close()

file2 = open(os.path.join(folder_name, "matrix_B.txt"), "w")
for row in matrix2:
    for num in row:
        file2.write(str(num) + " ")
    file2.write("\n")
file2.close()

```

verify.py:
```python
import numpy as np
import os

def read_matrix(filename):
    with open(filename, 'r') as f:
        matrix = []
        for line in f:
            row = [int(x) for x in line.split()]
            matrix.append(row)
    return np.array(matrix)

def main():
    base_path = "C:\\Users\\OrangeFruit\\Desktop\\parallel_programming\\lab1\\"

    sizes = [200, 400, 800, 1200, 1600, 2000]

    print("="*60)
    print("ВЕРИФИКАЦИЯ РЕЗУЛЬТАТОВ ПЕРЕМНОЖЕНИЯ МАТРИЦ")
    print("="*60)

    all_passed = True

    for n in sizes:
        folder_path = os.path.join(base_path, f"matrix_size_{n}")

        matrix_a_path = os.path.join(folder_path, "matrix_A.txt")
        matrix_b_path = os.path.join(folder_path, "matrix_B.txt")
        result_path = os.path.join(folder_path, "result.txt")

        if not all(os.path.exists(p) for p in [matrix_a_path, matrix_b_path, result_path]):
            print(f"Пропуск размера {n}: не все файлы найдены")
            all_passed = False
            continue

        try:
            A = read_matrix(matrix_a_path)
            B = read_matrix(matrix_b_path)
            C = read_matrix(result_path)

            expected = np.dot(A, B)

            if np.array_equal(C, expected):
                print(f"РАЗМЕР {n:4d}: Верификация пройдена")
            else:
                print(f"РАЗМЕР {n:4d}: Верификация НЕ пройдена")
                all_passed = False

                diff = np.where(C != expected)
                mismatch_count = len(diff[0])
                print(f"   Найдено несовпадений: {mismatch_count}")

                if mismatch_count > 0:
                    print("   Все несовпадения:")
                    for idx in range(mismatch_count):
                        i, j = diff[0][idx], diff[1][idx]
                        print(f"      [{i}][{j}]: {C[i][j]} vs {expected[i][j]}")

        except Exception as e:
            print(f"РАЗМЕР {n:4d}: Ошибка при проверке - {e}")
            all_passed = False

    print("="*60)
    if all_passed:
        print("ИТОГ: Все результаты успешно верифицированы!")
    else:
        print("ИТОГ: Обнаружены ошибки в результатах")
    print("="*60)

if __name__ == "__main__":
    main()
```


5. Данные отработки модуля верификации:  
| Размер | Статус |
|--------|--------|
| 200 | Верификация пройдена |
| 400 | Верификация пройдена |
| 800 | Верификация пройдена |
| 1200 | Верификация пройдена |
| 1600 | Верификация пройдена |
| 2000 | Верификация пройдена |  
6. Данные отработки программы для вычисления произведения матриц:
```
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
```

7. Таблица результатов перемножения:
| Размер | Время (мкс) | Время (мс) | Операций |
|--------|-------------|------------|----------|
| 200 | 46041 | 46.041 | 16000000 |
| 400 | 393719 | 393.719 | 128000000 |
| 800 | 3558508 | 3558.508 | 1024000000 |
| 1200 | 11794371 | 11794.371 | 3456000000 |
| 1600 | 30652877 | 30652.877 | 8192000000 |
| 2000 | 83906604 | 83906.604 | 16000000000 |  

8. Графики зависимостей:
![Графики зависимостей](https://github.com/andruyxaParavoz/parallel-programming/blob/lab1/matrix_analysis.png)

9. Вывод:
В ходе выполнения лабораторной работы были созданы: модуль для генерации двух квадратных матриц matrix_generator.py, программа вычисления произведения двух квадратных матриц и записи результата в конечный файл result.txt, модуль для верификации вычислений verify.py с помощью библиотеки NumPy. Программа исправно выполняет вычисления и проходит проверку с помощью вышеописанного модуля. Алгоритмическая сложность эксперементально доказана с помощью тестов алгоритма перемножения матриц разного размера.


