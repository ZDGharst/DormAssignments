### Dorm Assignments
This program assigns two hundred roommates to fifty 4-person rooms using simulated annealing.

Receives input from the file at **data/roommates.txt**. Input is expected to be 200 rows of 200 integer values separated by spaces.

Type "make" in the terminal in the project folder to compile and run the project.

Program by Zach Gharst for CS461 - Introduction to Artificial Intelligence at UMKC.

## Optimizations:
Originally, the input file is a 200x200 matrix. However, the diagonal is all zeroes, and the values across the diagonal are symmetric to each other. As in, for some arbitrary value **x**, matrix[x][x] is always equal to zero. For some arbitrary **x** and **y**, matrix[x][y] == matrix[y][x]. Instead of storing 40000 values, we only have to store 40000/2 - 200 = 19800 values.

Here's the naive approach where all 40000 values are stored:

    int compatibilities[40000];
    std::ifstream compatibilityFile;
    compatibilityFile.open("data/roommates.txt");

    for(int i = 0; i < 40000; i++) {
        compatibilityFile >> compatibilities[i];
    }

    compatibilityFile.close();
    
    
