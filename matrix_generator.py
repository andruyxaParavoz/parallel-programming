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
