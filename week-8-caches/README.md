# Matrix Manipulation and Caches

## Description 

Beginning with starter code `matrices.c` (which initializes matrixes for you), implement functions for  matrix addition (adding two 2D matrices of equal size) and matrix transposition of square matrixes (swap elements around the diagonal).  Use the following criteria:

* use double precision floating point and dynamically allocated arrays (done in starter code)
* write helper functions to initialize each matrix with appropriately randomized numbers (elaborate on starter code)
* time each *serial* function using the OpenMP timing functions across a range of matrix sizes (be sure not to time your randomization routines):
    * Matrix Sizes: 100x100, 500x,1000x,2000x, (larger if time/space allow)
* Always use square matrices! Much easier!

## Parallelizing 

Now Parallelize your matrix operations using OpenMP

* time each implementation varying each of the following:
    * number of threads
    * matrix size
    * schedule, using the OpenMP `schedule` pragma directive
        *   static, chunksizes of 1, 10, and 100
        * dynamic, chunksizes of 1, 10, 100

**Hint**: Use a non-random seed to initialize the matrix, so that you can confirm that your single and multi-threaded programs produce identical results.

## Crowdsourcing results


I will have created a google spreadsheet for you all to post timings for the various experiments.  Please post your data, and be sure to indicate what processor architecture you are using.

## Submit 

An analysis of the Speedup and Efficiency of your code.   In cases where speedup and efficiency don't scale linearly, provide some explanations about how interaction between caches and cores could affect your measurements

Add/Commit/Push:
*  your code
* any additional code/scripts used for testing
* your `Writeup.md` (submitted here, via nexus)