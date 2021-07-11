# <center> Week 8 Assignments </center>
# <center> Di Wu </center>

### Tasks 

- [x] Serial matrix addition and transpose.
- [x] Static scheduling matrix addition and transpose.
- [x] Dynamic scheduling matrix addition and transpose.

### Speedup Analysis

The speedup is measured by the wall timer `omp_get_wtime()`.

#### Matrix Addition
All combiniations of 

- Matrix of size: 2000, 8000
- Number of threads: 1(serial), 4, 10
- Chunk size: 1, 10, 100

are tested.

For **static** scheduling:

- Matrix size: 2000

    - Chunk size: 1

    | # Threads  | Execution Time | Speed Up |
    |------------|----------------|----------|
    | 1 (serial) | 0.024691s      | 1        |
    | 4          | 0.010021s      | 2.46     |
    | 10         | 0.012827s      | 1.92     |

    - Chunk size: 10

    | # Threads  | Execution Time | Speed Up |
    |------------|----------------|----------|
    | 1 (serial) | 0.027234s      | 1        |
    | 4          | 0.009432s      | 2.88     |
    | 10         | 0.012095s      | 2.25     |

   - Chunk size: 100

    | # Threads  | Execution Time | Speed Up |
    |------------|----------------|----------|
    | 1 (serial) | 0.025398s      | 1        |
    | 4          | 0.009365s      | 2.71     |
    | 10         | 0.012134s      | 2.09     |

    - *Analysis*: Given the settings of static schedule and matrix size 2000, we can observe how number of threads and chunk size affect the speedup and the execution time. Given the same chunck size, we notice that 4-thread program has a larger speedup than the 10-thread, probably because of the medium matrix size. Currently, we do not see the impact of the chunk size on the speed up.

- Matrix size: 8000

    - Chunk size: 1

    | # Threads  | Execution Time | Speed Up |
    |------------|----------------|----------|
    | 1 (serial) | 0.565992s      | 1        |
    | 4          | 0.144999s      | 3.90     |
    | 10         | 0.175428s      | 3.22     |

    - Chunk size: 10

    | # Threads  | Execution Time | Speed Up |
    |------------|----------------|----------|
    | 1 (serial) | 0.445936s      | 1        |
    | 4          | 0.159337s      | 2.79     |
    | 10         | 0.172383s      | 2.58     |

   - Chunk size: 100

    | # Threads  | Execution Time | Speed Up |
    |------------|----------------|----------|
    | 1 (serial) | 0.425152s      | 1        |
    | 4          | 0.141487s      | 3.00     |
    | 10         | 0.166405s      | 2.55     |

For **dynamic** scheduling:

- Matrix size: 2000

    - Chunk size: 1

    | # Threads  | Execution Time | Speed Up |
    |------------|----------------|----------|
    | 1 (serial) | 0.026988s      | 1        |
    | 4          | 0.010756s      | 2.50     |
    | 10         | 0.017597s      | 1.53     |

    - Chunk size: 10

    | # Threads  | Execution Time | Speed Up |
    |------------|----------------|----------|
    | 1 (serial) | 0.02705s       | 1        |
    | 4          | 0.009426s      | 2.86     |
    | 10         | 0.011907s      | 2.27     |

   - Chunk size: 100

    | # Threads  | Execution Time | Speed Up |
    |------------|----------------|----------|
    | 1 (serial) | 0.027163s      | 1        |
    | 4          | 0.009281s      | 2.92     |
    | 10         | 0.012138s      | 2.23     |

- Matrix size: 8000

    - Chunk size: 1

    | # Threads  | Execution Time | Speed Up |
    |------------|----------------|----------|
    | 1 (serial) | 0.428808s      | 1        |
    | 4          | 0.14624s       | 2.93     |
    | 10         | 0.187899s      | 2.28     |

    - Chunk size: 10

    | # Threads  | Execution Time | Speed Up |
    |------------|----------------|----------|
    | 1 (serial) | 0.437819s      | 1        |
    | 4          | 0.141279s      | 3.09     |
    | 10         | 0.176631s      | 2.47     |

   - Chunk size: 100

    | # Threads  | Execution Time | Speed Up |
    |------------|----------------|----------|
    | 1 (serial) | 0.401325s      | 1        |
    | 4          | 0.157664s      | 2.54     |
    | 10         | 0.290565s      | 1.38     |


- *Analysis*: Now we expect to see some correlations between the speedup and the scheduling type. The dynamic scheduling should have less execution time given other settings the same because the core can ask for more task if it finishes the assigned task. In contrast, the task is distributed before the loop in the static scheduling. However, we do not observe the reduction on execution time by the dynamic scheduling. The unexpected result may be caused by distribution of the task. When the task is evenly distributed, all threads are likely to finished at the same time. Hence the advantage of dynamic is not obvious.

#### Matrix Transpose

All combiniations of 

- Matrix of size: 2000, 8000
- Number of threads: 1(serial), 4, 10
- Chunk size: 1, 10, 100

are tested.

For **static** scheduling:

- Matrix size: 2000

    - Chunk size: 1

    | # Threads  | Execution Time | Speed Up |
    |------------|----------------|----------|
    | 1 (serial) | 0.025133s      | 1        |
    | 4          | 0.025063s      | 1.00     |
    | 10         | 0.042906s      | 0.58     |

    - Chunk size: 10

    | # Threads  | Execution Time | Speed Up |
    |------------|----------------|----------|
    | 1 (serial) | 0.026146s      | 1        |
    | 4          | 0.01635s       | 1.59     |
    | 10         | 0.022759s      | 1.14     |

   - Chunk size: 100

    | # Threads  | Execution Time | Speed Up |
    |------------|----------------|----------|
    | 1 (serial) | 0.023643s      | 1        |
    | 4          | 0.012104s      | 1.95     |
    | 10         | 0.022505s      | 1.05     |

- Matrix size: 8000

    - Chunk size: 1

    | # Threads  | Execution Time | Speed Up |
    |------------|----------------|----------|
    | 1 (serial) | 0.748937s      | 1        |
    | 4          | 0.445165s      | 1.68     |
    | 10         | 0.769119s      | 0.97     |

    - Chunk size: 10

    | # Threads  | Execution Time | Speed Up |
    |------------|----------------|----------|
    | 1 (serial) | 0.647026s      | 1        |
    | 4          | 0.413902s      | 1.56     |
    | 10         | 0.383089s      | 1.68     |

   - Chunk size: 100

    | # Threads  | Execution Time | Speed Up |
    |------------|----------------|----------|
    | 1 (serial) | 0.652033s      | 1        |
    | 4          | 0.241261s      | 2.70     |
    | 10         | 0.320370s      | 2.03     |

For **dynamic** scheduling:

- Matrix size: 2000

    - Chunk size: 1

    | # Threads  | Execution Time | Speed Up |
    |------------|----------------|----------|
    | 1 (serial) | 0.026681s      | 1        |
    | 4          | 0.025285s      | 1.05     |
    | 10         | 0.043251s      | 0.61     |

    - Chunk size: 10

    | # Threads  | Execution Time | Speed Up |
    |------------|----------------|----------|
    | 1 (serial) | 0.026735s      | 1        |
    | 4          | 0.015428s      | 1.73     |
    | 10         | 0.022790s      | 1.17     |

    - Chunk size: 100

    | # Threads  | Execution Time | Speed Up |
    |------------|----------------|----------|
    | 1 (serial) | 0.026751s      | 1        |
    | 4          | 0.012485s      | 2.14     |
    | 10         | 0.020798s      | 1.28     |

- Matrix size: 8000

    - Chunk size: 1

    | # Threads  | Execution Time | Speed Up |
    |------------|----------------|----------|
    | 1 (serial) | 0.690546s      | 1        |
    | 4          | 0.480394s      | 1.43     |
    | 10         | 0.871398s      | 0.79     |

    - Chunk size: 10

    | # Threads  | Execution Time | Speed Up |
    |------------|----------------|----------|
    | 1 (serial) | 0.669566s      | 1        |
    | 4          | 0.399045s      | 1.67     |
    | 10         | 0.429700s      | 1.55     |

   - Chunk size: 100

    | # Threads  | Execution Time | Speed Up |
    |------------|----------------|----------|
    | 1 (serial) | 0.634s         | 1        |
    | 4          | 0.264708s      | 2.39     |
    | 10         | 0.376996s      | 1.68     |

- **Analysis**: 
For the matrix transpose, we can still observe that in general 4-thread program has a larger speedup than the 10-thread program. For some experiments, the speedup of the 10-thread program is even less than 1, which may indicate that the workload of transpose is not heavy enough to initialize the 10-thread program. 
However, a special case exists that given the static scheduling, 8000 matrix size, and 10 chunk size, 10 threads have speedup 1.68, which is greater than the speedup of the 4-thread program 1.56. It is possible that the 4-thread program is undergoing more cache miss than the 10 threads program.


Matrix Manipulation and Caches 55/60
- [] Code Exists
- [] Code Compiles
- [] Code Runs
- [] Writeup and Analysis

* I'd like to see in-place transpose timing
* I'd like to see 24+48 thread results on larger matrixes on skylake