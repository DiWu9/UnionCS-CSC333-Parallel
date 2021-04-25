# <center> Week 4 Assignments </center>
# <center> Di Wu </center>

## Normalize Vector (AllReduce)

### Tasks 

- [x] Use `MPI_Allreduce` with the `MPI_Sum` operator to get the sum of squares shared with *all* tasks
- [x] Wall Timing and Speedup Analysis

### Speedup Test

*Note*: The speedup equals to the execution time of the program running serially divided by the execution time of the root-task running on parallel. The speedup and the execution time of the program running on various vector sizes are shown below:

#### Vector Size 12:

Using `Scatter` and `Broadcast` to compute magnitude:

| # Processors | Execution Time | Speed Up |
| ------------ | -------------- | -------- |
| 1 (serial) | 0.05ms | 1 |
| 2 | 0.26ms | 0.19 |
| 3 | 0.31ms | 0.16 |
| 4 | 0.35ms | 0.14 |

Using `Allreduce` with `MPI_Sum` to get sum of squares and magnitude:

| # Processors | Execution Time | Speed Up |
| ------------ | -------------- | -------- |
| 1 (serial) | 0.05ms | 1 |
| 2 | 0.27ms | 0.19 |
| 3 | 0.29ms | 0.17 |
| 4 | 0.43ms | 0.12 |

- *Comment:* 
No significant difference between parallel programs in both versions when normalizing a 12-element array. The parallel program still takes more time to finish than the serial program.

#### Vector Size 240:

Using `Scatter` and `Broadcast` to compute magnitude:

| # Processors | Execution Time | Speed Up |
| ------------ | -------------- | -------- |
| 1 (serial) | 0.25ms | 1 |
| 2 | 0.50ms | 0.50 |
| 3 | 0.56ms | 0.45 |
| 4 | 0.87ms | 0.29 |

Using `Allreduce` with `MPI_Sum` to get sum of squares and magnitude:

| # Processors | Execution Time | Speed Up |
| ------------ | -------------- | -------- |
| 1 (serial) | 0.19ms | 1 |
| 2 | 0.59ms | 0.32 |
| 3 | 0.62ms | 0.31 |
| 4 | 0.61ms | 0.31 |

- *Comment:* 
`Allreduce` is faster in both serial program and 4-processor parallel program, and `Scatter` and `Broadcast` is faster in 2-processor program and 3-processor program. The serial prorgam should take the same time to execute despite of the difference of the MPI collective functions applied in program. Hence by the information above we still cannot distinguish which MPI method is more efficient.

#### Vector Size 1200:

Using `Scatter` and `Broadcast` to compute magnitude:

| # Processors | Execution Time | Speed Up |
| ------------ | -------------- | -------- |
| 1 (serial) | 0.57ms | 1 |
| 2 | 1.27ms | 0.45 |
| 3 | 0.92ms | 0.62 |
| 4 | 0.95ms | 0.60 |

Using `Allreduce` with `MPI_Sum` to get sum of squares and magnitude:

| # Processors | Execution Time | Speed Up |
| ------------ | -------------- | -------- |
| 1 (serial) | 0.53ms | 1 |
| 2 | 1.04ms | 0.51 |
| 3 | 0.82ms | 0.65 |
| 4 | 0.90ms | 0.59 |

*Comment:* Given the same number of processors, the execution time of `MPI_Allreduce` is less than the execution time of `Scatter` and `Broadcast`. Hence `Allreduce` has more efficiency than `Scatter` and `Broadcast` when spiltting large-size vetors.


## Vector Mean

Generate random vector of a given size (or default 12) and calculate the average of the elements.

### Tasks 

- [x] Each task creates array of random values and computes local sums. 
- [x] Root task use `MPI_Reduce` to compute global sum.

### Speed Up Test

#### Vector size 12:

| # Processors | Execution Time | Speed Up |
| ------------ | -------------- | -------- |
| 1 (serial) | 0.04ms | 1 |
| 2 | 0.26ms | 0.15 |
| 3 | 0.21ms | 0.19 |
| 4 | 0.29ms | 0.14 |

#### Vector size 1200:

| # Processors | Execution Time | Speed Up |
| ------------ | -------------- | -------- |
| 1 (serial) | 0.06ms | 1 |
| 2 | 0.19ms | 0.32 |
| 3 | 0.17ms | 0.35 |
| 4 | 0.25ms | 0.24 |

#### Vector size 120000:

| # Processors | Execution Time | Speed Up |
| ------------ | -------------- | -------- |
| 1 (serial) | 2.02ms | 1 |
| 2 | 1.14ms | 1.77 |
| 3 | 0.96ms | 2.10 |
| 4 | 0.75ms | 2.69 |

- *Analysis:*
When the vector size is increasing, the speed up is increasing. When computing the average of the vector of size 12, the speed up of the parellel programs in 2-4 processors is in 0.14-0.19. When the vector size increase to 1200, the speed up of parallel programs in 2-4 processors increases to 0.24-0.32. But we still cannot observe the advantage of the parallel program. When the vector size becomes 120000, the speed up of parallel program now reaches 1.88-2.69, and each increment of the number of processors increases the speed significantly. Hence we may conclude that the advantage of the parallel program and the number of processors becomes increasingly obvious when the vector size (the amount of work) is increasing.

## Vector Standard Deviation

Generate random vector of a given size (or default 12) and calculate the standard deviation of the elements.

### Tasks 

- [x] Each task creates array of random values and computes local sums. 
- [x] Each task use `MPI_Allreduce` to compute global sum and average.
- [x] Each task compute local sum of square.
- [x] Root task use `MPI_Reduce` to compute global sum of square.
- [x] Root task print out the standard deviation (square root of the average of the gloabl sum of square).

### Speed Up Test

#### Vector size 12:

| # Processors | Execution Time | Speed Up |
| ------------ | -------------- | -------- |
| 1 (serial) | 0.05ms | 1 |
| 2 | 0.25ms | 0.20 |
| 3 | 0.38ms | 0.13 |
| 4 | 0.31ms | 0.16 |

#### Vector size 1200:

| # Processors | Execution Time | Speed Up |
| ------------ | -------------- | -------- |
| 1 (serial) | 0.67ms | 1 |
| 2 | 0.59ms | 1.18 |
| 3 | 0.55ms | 1.22 |
| 4 | 0.51ms | 1.31 |

#### Vector size 120000:

| # Processors | Execution Time | Speed Up |
| ------------ | -------------- | -------- |
| 1 (serial) | 35.63ms | 1 |
| 2 | 22.89ms | 1.56 |
| 3 | 21.90ms | 1.63 |
| 4 | 19.60ms | 1.82 |

- *Analysis:*
Because the vector std needs more computations than the vector average, the parallel starts to have speed up even when the vector size is 1200. The speed up of vector size 1200 also increases by the increment of the number of processors. The 2-processor program has the speed up of 1.18, the 3-processor program has the speed up of 1.22, and the speed up of the 4-processor program has 1.31. When the vector size reaches to 120000, the advantage of the parallel program and the number of processors becomes more significant. The 2-processor program has the speed up of 1.56, the 3-processor program has the speed up of 1.63, and the speed up of the 4-processor program has 1.82.