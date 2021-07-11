# Week 7 Programming Assignments: OpenMP


## Setup:
* `git pull` the class repo
* copy `week-7-openmp` into your personal repo

## Compiling notes:
* You can compile with `gcc -fopenmp` 
* Or you can just use the `Makefile` to `make pfor-no-sugar-arraysum`
    * Note: If you MacOS you'll have to modify the `Makefile`


# In-Class 1: Array Sum

Following a demonstration of how OpenMP handles parallel `pragma omp parallel` statements, and how scoping works for variables declared prior to and after forks. 


## Modify the code

Modify `pfor-no-sugar-arraysum.c` to (correctly) add the digits of the PiArray, following the prompts in the comments. Be sure to use a `critical` section where needed.  Run the code across multiple threads multiple times, to make sure you get the same answer every time.

# In-Class 2: Vector operations

* Now use OpenMP to parallelize the following vector functions found in `omp_vectors.c`
* You can compile with `make omp_vectors`


# Numerical Integration Estimation of Pi

In a file called `omp-pi.c`, following the pseudocode from lecture (and written below) estimate pi using numerical integration (Reimann Sums).  You'll need a very large `N` (and a very small `dx`).

Use a parallel for loop, with an addition reduction clause, and be mindful of variable scopes.

```
N = some large number
dx = 1/N
sum = 0
for i in range(1,N):
   w = dx 
   xval = i*dx
   h = sqrt(1-(xval)^2)
   sum += w*h

pi = 4*sum
```


