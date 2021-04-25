# <center>Week 2 Assignments</center>

# Tasks 

- [x] Array Sum 
- [x] Vector Dot
- [x] Vector Add 

# Observations and Measures

## Array Sum

**Algorithm**: 

The program applies MPI to block-wise distribute a big array to multiple processors for calculating the sum. For the block-wise distribution, the program first divides the array size by the number of processors to get the block size (quotient) `B` and the remainder `R`. Given the processor's rank `N`, the program assigns the block of start index `Start_i` as `N(B+1)` if `N <= R` and `NB+R` if `N>R`. The last index is `Start_i+B` if `N<R` and `Start_i+B-1` if `N>=R`.

**Result**:

| # Processors | 1st block sum | 2nd block sum | 3rd block sum | 4th block sum |
| ------------ | ------------- | ------------- | ------------- | ------------- |
| 1 | 1057 ||||
| 2 | 541 | 516 |||
| 3 | 368 | 339 | 350 ||
| 4 | 291 | 250 | 235 | 281 |

## Vector Dot

The root task generate 2 random vectors and send them to non-root tasks. All tasks compute the dot product by cyclic distribution. The root task collects the results and sum them together to get the dot product of two complete vectors.

**Speedup Test**: 

The speed up by the number of processors is tested and shown below. The execution time of parallel computing using N processors is the maximum execution time among the N processors.

- Array Size 10:

| # Processors | Execution Time | Speed Up |
| ------------ | -------------- | -------- |
| 1 (serial) | 60.064ms | 1 |
| 2 | 60.054ms | 1 |
| 3 | 62.889ms | 0.95 |
| 4 | 65.750ms | 0.91 |

- Array Size 10000:

| # Processors | Execution Time | Speed Up |
| ------------ | -------------- | -------- |
| 1 (serial) | 54.026ms | 1 |
| 2 | 61.955ms | 0.87 |
| 3 | 57.493ms | 0.94 |
| 4 | 61.637ms | 0.88 |

- Array Size 1000000:

| # Processors | Execution Time | Speed Up |
| ------------ | -------------- | -------- |
| 1 (serial) | 82.661ms | 1 |
| 2 | 98.664ms | 0.84 |
| 3 | 106.948ms | 0.77 |
| 4 | 130.092ms | 0.63 |

It is interesting to observe that the speed up of MPI parallel computing is always less than one, which means given the same vector size, the execution time of serial prorgam is always less than the execution time of the parallel program. The possible factors that cause this may include:
- MPI implementation: The root task sends the entire vectors to the non-root task (send before distritbution), which may be optimized by first distributing and then sending. I tried to send both vectors cyclicly to the non-root tasks but it still does not optimize the speed up because this process invokes a lot of `MPI_Send` and `MPI_Recv` function calls.
- Wrong Timing Technique: The timer compute the execution time of the main function, and the execution time of the parallel program is the main task that took the most time.

## Vector Add

The root task generates two random vectors and block distribute the index to non-root tasks. All tasks complete their vector addition and send their result back to the root task. The root task combine the answers to the complete result.

**Speedup Test**: 

The speed up by the number of processors is tested and shown below. The execution time of parallel computing using N processors is the maximum execution time among the N processors.

- Array Size 10:

| # Processors | Execution Time | Speed Up |
| ------------ | -------------- | -------- |
| 1 (serial) | 50.286ms | 1 |
| 2 | 61.697ms | 0.82 |
| 3 | 57.737ms | 0.88 |
| 4 | 58.927ms | 0.86 |

- Array Size 10000:

| # Processors | Execution Time | Speed Up |
| ------------ | -------------- | -------- |
| 1 (serial) | 56.173ms | 1 |
| 2 | 59.826ms | 0.94 |
| 3 | 60.909ms | 0.92 |
| 4 | 66.222ms | 0.85 |

- Array Size 1000000:

| # Processors | Execution Time | Speed Up |
| ------------ | -------------- | -------- |
| 1 (serial) | 354.077ms | 1 |
| 2 | 441.178ms | 0.80 |
| 3 | 381.482ms | 0.93 |
| 4 | 372.838ms | 0.95 |

# Feedback from John 

## Array Sum 9/10
- [] Code Exists
- [] Code Compiles
- [] Code Runs
- [] Writeup and Analysisj -1: great timing data, but missing some analysis
- Notes: 

## Vector Dot 9/10
- [] Code Exists
- [] Code Compiles
- [] Code Runs
- [] Writeup and Analysisj -1: great timing data, but missing some analysis
- Notes: 

##  Vector Add 4/5
- [] Code Exists
- [] Code Compiles
- [] Code Runs
- [] Writeup and Analysisj -1: great timing data, but missing some analysis
- Notes: 

Overall: this was a very nicely formatted writeup, and the timing data was much appreciated.  However I was hoping for some more detailed analysis of *why* you got no speedup.