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