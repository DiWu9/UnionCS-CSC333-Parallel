# <center> Week 2 Assignments</center>
<!-- TOC -->

- [<center> Week 2 Assignments</center>](#center-week-2-assignmentscenter)
- [Setup](#setup)
- [Hands-On (in-class Tuesday)](#hands-on-in-class-tuesday)
    - [Running MPI Hello](#running-mpi-hello)
    - [Running Array Sum](#running-array-sum)
    - [Modify Array Sum (10 points)](#modify-array-sum-10-points)
    - [Submit (Before Tuesday Week 3):](#submit-before-tuesday-week-3)
- [Vectorized Vector Operations](#vectorized-vector-operations)
    - [Setup](#setup)
- [Vectorizing Vector Dot Product  (10 points)](#vectorizing-vector-dot-product--10-points)
    - [Submit (Before Tuesday Week 3):](#submit-before-tuesday-week-3)
- [Vectorizing Vector Addition (Challenge): (5 points)](#vectorizing-vector-addition-challenge-5-points)
    - [Setup](#setup)
    - [Submit (Before Tuesday Week 3):](#submit-before-tuesday-week-3)

<!-- /TOC -->
---
# Setup

For starters I'll assume you have access to the following:
* a computer with `git` and `mpicc`.  `mpicc` is the compiler-tool for MPI.  *All* CS department machines have these installed
* that you have `git clone`d both the class repo and your personal repo onto this computer.

Begin by:
*  pulling the latest version of the class repo
*  copying the contents of `week-2-mpi` into your personal repo (via drag and drop or `cp -R`). Be sure to copy over `Writeup.md`
* copy the contents of `demos/MPI/` into your personal repo
* `git add` these new files.

---

# Hands-On (in-class Tuesday)

## Running MPI Hello
*  compile your copy of `mpi_hello.c` via:
    * `mpicc -g -Wall -std=c99 -o mpi_hello mpi_hello.c`
*  run it via:
    * `mpirun -np 4 ./mpi_hello`
* Try changing the number of processes by modifying the number after `-np`
---
## Running Array Sum
*  compile `MPI_array_sum.c` via:
    * `mpicc -std=c99 -Wall MPI_array_sum.c -o array_sum`
*  run it via:
    * `mpirun -np 4 ./array_sum`
* Try changing the number of processes by modifying the number after `-np`
---
## Modify Array Sum (10 points)

* Modify your copy of `MPI_array_sum.c` so that it distributes the data block-wise, rather than cyclically, using the algorithm you developed in class.  Write it scalably so that it works regardless of the number of processes (including just 1).
* This should mostly require writing a little bit of code before the `for` loop that calculates `localsum`, and then modifying the parameters of the `for` loop.
* You will need to acccount for situations when N%p!=0 (that is to say the size of the array is not evenly divisible by the number of processsors, you  )

## Submit (Before Tuesday Week 3):

* git add/commit/push your modified `MPI_array_sum.c`
* a Writeup.md describing your algorithm and results


# Vectorized Vector Operations

## Setup
* pull the class repo
* create a new MPI file, `MPI_vector_dot.c`.  I recommend you start with the template file in `demos/MPI`.
* add it to your repo in the `week-2-mpi` directory


# Vectorizing Vector Dot Product  (10 points)

* Now implement vector vector dot product in the corresponding `.c` file.  You can choose either a cyclic or block-based approach. (hint: use cyclic)
* The program should take as a command-line argument the size of the vectors to be using.
* The algorithm to follow is as follows:
    1. only the root task should generate the random vectors, and send them to the non-root tasks.
    2. the non-root tasks should receive the vectors from the root task
    3. then all tasks should perform their partial solutions
    4. then all non-root tasks send the result back to the root task.
    5. the root task should then receive the partial dot product solutions from the non-root tasks, and add them together.
    6. the root task should print the answer
* you can re-use the function from Week 1 that generates random arrays by cutting/pasting it into the `.c` files above `main()`
* Measure speedup for different vector sizes and numbers of processors. Make a note in `Writeup.md`

## Submit (Before Tuesday Week 3):
* git add/commit/push `MPI_Vector_dot.c`
* and your `Writeup.md`, addressing all questions above.


# Vectorizing Vector Addition (Challenge): (5 points)

## Setup
* pull the class repo
* create a new MPI file, `MPI_vector_add.c` in your personal repo .  I recommend you start with the template file in `demos/MPI`.
* add it to your repo in the `week-2-mpi` director


 ## Description

* Now implement vector vector addition in the corresponding `.c` files.  You *must* use a block-based approach.  

This is tricky to parallelize, but essentially each task will be responsible for the addition of only a slice of the big vectors.  The root task will be responsible for distributing slices to each task, and then gathering back each task's slice of the result.  

The steps are as follows:

* The program should take as a command-line argument the size of the vectors to be using.
* The algorithm to follow is as follows:
    1. only the root task should generate the random vectors. The root task should also allocate a result vector.
    2. all tasks should allocate three arrays for their "slice" of each vector (the two vectors to add and the result vector)
    3. the root task should send a slice to the corresponding tasks (but not itself)
    4. the non-root tasks should receive their slice from the root task
    5. all tasks should perform their partial solutions
    4. then all non-root tasks send their result slice back to the root task.
    5. the root task should then receive the result slices from the non-root tasks, and add them to the final result vecotr.
    6. the root task should print the answer
* you can re-use the function from Week 1 that generates random arrays by cutting/pasting it into the `.c` files above `main()`
* Note that much like problem `1.1` of the homework, not all slices will be equal!  Each task will need to calculate how big its slice is.
* Measure speedup for different vector sizes and numbers of processors. Make a note in `Writeup.md` 

## Submit (Before Tuesday Week 3):
* git add/commit/push `MPI_Vector_add.c`
* and your `Writeup.md`, addressing all questions above.