# <center> Week 7 Assignments </center>
# <center> Di Wu </center>

### Tasks 

- [x] Array Sum.
- [x] Vector Operations.
- [x] Estimation of Pi.

### Speedup Analysis

The speedup is measured by the wall timer `omp_get_wtime()`.

#### Array Sum

To test the speedup of array sum on array with larger size, I add the function `normal_vector` to generate an arrary of ones.

- Array size: 1000000

| # Threads | Time to Sum | Speed Up |
| ------------ | ----------- | -------- |
| 1 (serial) | 1.82ms | 1 |
| 10 | 1.36ms | 1.33 |
| 20 | 2.17ms | 0.83 |
| 40 | 2.88ms | 0.63 |

- Array size: 100000000

| # Threads | Time to Sum | Speed Up |
| ------------ | ------------- | -------- |
| 1 (serial) | 149.87ms | 1 |
| 10 | 69.96ms | 2.15 |
| 20 | 135.13ms | 1.11 |
| 40 | 252.77ms | 0.59 |

- **Analysis**: When computing the sum of an array of size 1000000, the speedup of parallel program is greater than 1 only when using 10 threads. When number of threads was increased to 20 and 40, the speedup was decreasing. Similarly, when the size of the array increased to 100000000, the speedup of 10-thread parallel program has the maximum speedup compared with 20 threads and 40 threads. Because there are no overhead of the data communication among shared-memory mutiprocessors, I assume the increment of execution time was caused by the aggregating the localsum of each processor to the globalsum (this processor cannot happen in parallel). The number of processors has a linear positive relationship with the aggregation time.

#### Vector Normalization

- Array size: 1000000

| # Threads | Time to Sum | Speed Up |
| ------------ | ----------- | -------- |
| 1 (serial) | 26.68ms | 1 |
| 10 | 9.31ms | 2.86 |
| 20 | 8.30ms | 3.21 |
| 40 | 8.43ms | 3.16 |

- Array size: 100000000

| # Threads | Time to Sum | Speed Up |
| ------------ | ------------- | -------- |
| 1 (serial) | 2586.96ms | 1 |
| 10 | 611.54ms | 4.23 |
| 20 | 593.26ms | 4.36 |
| 40 | 587.04ms | 4.41 |

- **Analysis**: For vector normalization of the array of size 1000000, the speedup increases when the number of threads increase from 1 (serial) to 10, and 10 to 20. The speedup decreases a little when the number of threads increases from 20 to 40. Hence we can conclude that the task is not large enough to be executed with 40 threads. The the array size increases to 100000000, the task is large enough so that 40-thread gains the max speedup.


### Estimation of Pi

The estimation of Pi reaches to the accurate state (Pi = 3.141593) when N = 2362020.

- N = 1000000

| # Threads | Time to Sum | Speed Up |
| ------------ | ------------- | -------- |
| 1 (serial) | 41.40ms | 1 |
| 10 | 16.21ms | 2.55 |
| 20 | 13.45ms | 3.08 |
| 40 | 13.96ms | 2.97 |

- N = 100000000

| # Threads | Time to Sum | Speed Up |
| ------------ | ------------- | -------- |
| 1 (serial) | 4022.92ms | 1 |
| 10 | 567.93ms | 7.08 |
| 20 | 606.42ms | 6.63 |
| 40 | 581.81ms | 6.91 |

- **Analysis**:  When N = 1000000, the speedup increases when the number of threads increases from 1 to 10 and 10 to 20. There is a little drop of the speedup when the number of threads increases from 20 to 40. When N = 100000000, the speedup does not increase when the number of threads increases from 10 to 20, but increases when the number of threads increases from 20 to 40. It is interesting because when the amount of task increases, more threads should get more speedup. The reason why more threads do not get more speedup might be
    - The fluctuation of the wall timer or the execution time.
    - The time cost by reduction of the Riemann Sum.