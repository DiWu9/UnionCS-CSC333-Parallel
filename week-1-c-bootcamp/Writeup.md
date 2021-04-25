
<center>Di Wu (Woody)</center>

# Task List
- [x] Complete Setup (clone repos)
- [x] Assignment 1: Factorization
- [x] Assignment 2: Vectors and Pointers 
- [x] Assignment 3: Linked List

# Assignment 1: Modifying factors

1. Check prime and find the largest divisor (Before Optimization)

Given a big number N, the program checks whether N is a prime number by iterating from 3 to N/2 with an increment of 1 and checking if N can be divided by the number. If N is a prime number, the message that N is a prime number will be printed. Otherwise, the message of N is not a prime number and its largest factor will be printed.

| Experiment # | Number | Largest Factor | Run Time (ms) |
| ----------- | ----------- | ----------- | ----------- |
| 1 | 26419097 | 83341 | 37.853 |
| 2 | 26419097 | 83341 | 50.652 |
| 3 | 26419097 | 83341 | 37.628 |
| 4 | 73856093 | 1 (prime) | 104.112 |
| 5 | 73856093 | 1 (prime) | 102.843 |
| 6 | 73856093 | 1 (prime) | 103.598 |

2. Check prime and find the largest divisor (After Optimization)

A more efficient prime checker of a big integer N is to iterate from 3 to sqrt(N) with an increment of 2.

| Experiment # | Number | Run Time (ms) |
| ----------- | ----------- | ----------- |
| 1 | 26419097 | 0.022 |
| 2 | 26419097 | 0.021 |
| 3 | 26419097 | 0.04 |
| 4 | 73856093 | 0.034 |
| 5 | 73856093 | 0.046 |
| 6 | 73856093 | 0.034 |

3. Why get different time values for the same number?

The run time of checking the same number decreases after the optimization because running time reduces from O(n) to O(sqrt(n)). The run time values are different between experiments that have the same number running on the same program. Because the program computes the run time by dividing the total number of clock cycles by the constant of number of clock cycles per second, the fluctuation in run time may be affected by the fluctuation of clock cycles, which is possible because in different time, the operating system is dealing with different tasks and the time per clock cycle also varies based on the task it is currently dealing with.

# Assignment 2: Randomizing vectors

1. How `argc` and `argv` work?

+ `argc`: specify the number of arguements in the command line.
+ `argv`: the array of arguements in string in the command line.

2. What happen if the vector size arguement and the actual size of the vector are different?

+ When **read** out of bounds (W < R): The printed vector is still in the specified read size R, but the values out of bounds are 0s.
+ When **write** out of bounds (W > R): The vector of size W is successfully written to the memory, but the program only prints the initial R elements of the vector.

3. Timing

+ It took 3207.805ms to generate two vectors of size 100,000,000 and compute their sum and dot product.
+ It took 112995.049ms to generate two vectors of size 1,000,000,000 and compute their sum and dot product.
+ The maximum size of two vectors the program can generate is 1,410,065,408 because when I set size arguement to 10,000,000,000, the program only generate two vectors of size 1,410,065,408 (I assume the data memory reaches its limit). The execution time of the program, including generating two vectors, computing their addition, and computing their dot product, is 142279.244ms.

# Assignment 3: Linked List

1. Description

The linked list is constructed by a series of simple linked nodes. The `Node` data structure contains an int value and a pointer to the next node. Both functions, `insert` and `printList`, are implemented using both loops and recursions. 


# Feedback

Factors: 10/10
- [ ] Code Exists
- [x] Code Compiles
- [x] Code Runs
- [x] Writeup and Analysis
- Notes: 

Vectors: 10/10
- [x] Code Exists
- [x] Code Compiles
- [x] Code Runs
- [x] Writeup and Analysis
- Notes:  your iteration using offset is unconventional, but works.

Linked-List: 5/5
- [x] Code Exists
- [x] Code Compiles
- [x] Code Runs
- [x] Writeup and Analysis
- Notes: 