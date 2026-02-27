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