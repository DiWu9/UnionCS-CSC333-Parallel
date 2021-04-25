# <center>Week 3 Assignments</center>

## Normalize Vector

### Tasks 

- [x] Normalize Vector
- [x] Parallelized Compute Magnitude
- [x] Wall Timing and Speedup Analysis

The program calculates the norm of a vector using `MPI_Bcast`, `MPI_Scatter` and `MPI_Gather`. The workflow of the program is shown below:

1. Generate vector on root (make sure N%p == 0)
2. scatter vector  to all nodes
3. all nodes calculate local sum-of-squares
4. all nodes send local sum back to root, root receives
5. root accumulates local sums, takes square root
6. root broadcasts sqrt to nodes
7. all nodes normalize their local vector
8. all nodes gather back to root node

**Speedup Test:**

The speedup equals to the execution time of the program running serially divided by the execution time of the root-task running on parallel. The speedup and the execution time of the program running on various vector sizes are shown below:

- Vector Size 12:

| # Processors | Execution Time | Speed Up |
| ------------ | -------------- | -------- |
| 1 (serial) | 0.05ms | 1 |
| 2 | 0.26ms | 0.19 |
| 3 | 0.31ms | 0.16 |
| 4 | 0.35ms | 0.14 |

Based on the data shown above, we can see the program calculating the norm of the vector of size 12 serially is much faster than the parallel program. The parallel program takes extra time for data communication among processors. The workload of processors in parallel program is trivially reduced because 12 is already a small vector size.

- Vector Size 240:

| # Processors | Execution Time | Speed Up |
| ------------ | -------------- | -------- |
| 1 (serial) | 0.25ms | 1 |
| 2 | 0.50ms | 0.50 |
| 3 | 0.56ms | 0.45 |
| 4 | 0.87ms | 0.29 |

The speedup running on the vector of size 240 is also less than 1, which means the serial program consumes less time than the parallel programs. The case of running on 4 processors consumes the most time because the data communication is more frequent.

- Vector Size 1200:

| # Processors | Execution Time | Speed Up |
| ------------ | -------------- | -------- |
| 1 (serial) | 0.57ms | 1 |
| 2 | 1.27ms | 0.45 |
| 3 | 0.92ms | 0.62 |
| 4 | 0.95ms | 0.60 |

Although there is no speedup of the parallel version compared with the serial one, the parallel program running on 3 and 4 processors is more time-efficient than the program running on 2 processors. Hence we can see that even though it takes more time for data communication for more processors, an increment on the number of processors does optimize the running time because the work is split up by processors when the vector is too large to be handled by a single processor.



## Parallelizing a 1-D Cellular Automaton

### Tasks 

- [x] Scatter the world
- [x] Neighbor Communication
- [x] Gather the world
- [x] Add parameters
- [x] Print cellworld using "o" and "."

### Description of Neighbor Communication Scheme:

**Goal**: When the cellworld is scattered to multiple processors, the cells on the edges need the their neighbor cells to update. Hence each processor have to send their edge cells to its right and left processors and also receive adjacent cells from them. 

**Implementation**: I let each processors send their left cell to their left processors (if `id == 0`, then `left_id = num_processors - 1`). Then each processor should receive a cell from its right processor and this should be their right extended cell. Then each processors send their right cell to their right processors (if `id == num_processors - 1`, then `right_id = 0`).Finally, each processor receive the send from its left processor and store it as its left extended value. Hence now each local block know the left cell of its right neighbor block and the right cell of its left neighbor block.

**Speedup accross worldsize**:

The speedup was measured in worldsize: 128, 152, 400, and 800. The execution time is recorded by the wall time (provided by MPI_Wtime), and execution time of the parallel program is recorded as the execution time of the root task.

- CA Rule: 30; Max iteration: 100

|   Worldsize  |    Serial      |        x2 Processors    |       x4 Processors     |
| ------------ | -------------- | ----------------------- | ----------------------- |
|     128      |       1.46ms   | 3.49ms (Speedup: 0.42)  | 3.30ms (Speedup: 0.44)  |
|     152      |       2.07ms   | 3.07ms (Speedup: 0.67)  | 4.00ms (Speedup: 0.52)  |
|     400      |       4.61ms   | 5.28ms (Speedup: 0.87)  | 6.26ms (Speedup: 0.74)  |
|     800      |       8.05ms   | 7.48ms (Speedup: 1.08)  | 9.79ms (Speedup: 0.82)  |

When the worldsize is increasing, the execution time of both serial and parallel is increasing, as expected. Both two-processor parallel and four-processor parallel have the lowest speedup in worldsize 128. When the worldsize is increasing, the speedup of both parallel programs is increasing. When the worldsize is 800, two-processor program becomes faster than the serial program. The reason why both parallel programs are slower given a small worldsize is that the time saved by the parallel is less than the time spent by the scatter, gather, and neighbor communication scheme. The four-processor program needs more time for scatter, gather, and neighbor commincation than the two-processor program. The number of both `MPI_Send` and `MPI_Recv` calls in neighbor communication is 4 in two-processor program, which is a half of the send and receive in four-processor program.