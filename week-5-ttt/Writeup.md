# <center> Week 5 Assignments </center>
# <center> Di Wu </center>

## Minimax Tic Tac Toe Search

### Tasks 

- [x] In the computer move, cyclic distribute the empty boxes to tasks and take the move of the best score.
- [x] Wall Timing and Speedup Analysis.

### Speedup Test

In the ``3x3`` tic tac toe board:

The speed up of computer being the ``1st player``:

To minimize the number of external variables that affect the speedup, the player takes the fixed sequence of move: 4,2,3,7; the computer takes the fixed move 0,1,6,5,8. 

- How fast does a computer make a move?

| # Processors | 1st move | 3rd move | 5th move | 7th move | 9th move | Average |
| ------------ | -------- | -------- | -------- | -------- | -------- | -------- |
| 1 (serial) | 52.17ms | 1.62ms | 0.06ms | 0.01ms | 0.00ms | 10.77ms |

| # Processors | 1st move | 3rd move | 5th move | 7th move | 9th move | Average |
| ------------ | -------- | -------- | -------- | -------- | -------- | -------- |
| 2 | 19.85ms | 0.35ms | 0.03ms | 0.02ms | 0.02ms | 4.05ms |

| # Processors | 1st move | 3rd move | 5th move | 7th move | 9th move | Average |
| ------------ | -------- | -------- | -------- | -------- | -------- | -------- |
| 3 | 12.66ms | 0.26ms | 0.04ms | 0.03ms | 0.05ms | 2.07ms |

| # Processors | 1st move | 3rd move | 5th move | 7th move | 9th move | Average |
| ------------ | -------- | -------- | -------- | -------- | -------- | -------- |
| 4 | 12.74ms | 0.24ms | 0.04ms | 0.03ms | 0.03ms | 2.62ms |

- The speedup of average computer move:

| # Processors | Avg ms to make a move | Speed Up |
| ------------ | --------------------- | -------- |
| 1 (serial) | 10.77ms | 1 |
| 2 | 4.05ms | 2.66 |
| 3 | 2.07ms | 5.20 |
| 4 | 2.62ms | 4.11 |

- Anaylsis:

According to tables about the milli-seconds the computer takes to make a single move, we can observe that computer takes more time to think if there are more empty boxes, which is expected because each empty space trigger a minimax recursion to get the best score that the computer can get by filling this box. The time spent on the 3rd move is fundamentally decreased compared to the time spent on the 1st move, and similarly, the time of the 5th move is decreased compared to the 3rd move, etc. 

By comparing the speedup of the average time spent on each move, all non-serial programs get significant speedup. But it is interesting to notice that the 3-processor program gets more speedup than the 4-processor program. I assume this has to do with the size of the board (9 is divisible by 3). Given that the computer is making the first move, there are 9 possible moves for the first move, and each processor compute the score of 3 moves. In contrast, 9 moves cannot be evenly distributed to 4 tasks, and the root task still get 3 moves to process. Because 4 tasks needs more time for communication, the speedup of 4-processor program is less than the speedup of 3-processor program when the computer makes the 1st move.

The speed up of compute being the ``2nd player``:

To minimize the number of external variables that affect the speedup, the player takes the fixed sequence of move: 4,2,3,7,8; the computer takes the fixed move 0,6,5,1. 

- How fast does a computer make a move?

| # Processors | 2st move | 4rd move | 6th move | 8th move | Average |
| ------------ | -------- | -------- | -------- | -------- | --------- |
| 1 (serial) | 9.06ms | 0.32ms | 0.02ms | 0.03ms | 2.36ms |

| # Processors | 2st move | 4rd move | 6th move | 8th move | Average |
| ------------ | -------- | -------- | -------- | -------- | --------- |
| 1 (serial) | 2.29ms | 0.11ms | 0.09ms | 0.05ms | 0.635ms |

| # Processors | 2st move | 4rd move | 6th move | 8th move | Average |
| ------------ | -------- | -------- | -------- | -------- | --------- |
| 1 (serial) | 1.70ms | 0.07ms | 0.03ms | 0.03ms | 0.46ms |

| # Processors | 2st move | 4rd move | 6th move | 8th move | Average |
| ------------ | -------- | -------- | -------- | -------- | --------- |
| 1 (serial) | 1.43ms | 0.06ms | 0.03ms | 0.03ms | 0.39ms |

- The speedup of average computer move:

| # Processors | Avg ms to make a move | Speed Up |
| ------------ | --------------------- | -------- |
| 1 (serial) | 2.36ms | 1 |
| 2 | 0.635ms | 3.72 |
| 3 | 0.46ms | 5.13 |
| 4 | 0.39ms | 6.05 |

- Anaylsis:

When the computer takes the 2nd move, we can observe that the computer takes less time to think than the time spent on the 1st move because there are less boxes to be recursively minimaxed. Hence the average time if computer takes the 2nd move is less than the average time the computer takes the 1st move.

According to the table suggesting the average speedup, we notice that all non-serial programs get significant speedup. The case that the 3-processor is faster than the 4-processor vanishes because when the computer takes the 2nd move, there are 8 possible moves that need to be minimaxed. The number is divisible by 4 instead of 3.

In general, all parallel programs get speedup from minimaxing the tic tac toe game. If we know the computer is fixed to make the first move, it's better to use 3 processors to get the maximum speedup if the maximum number of processors is 4. If the computer is fixed to make the second move, it's better to use 4 processors  to get the maximum speedup if the maximum number of processors is 4.