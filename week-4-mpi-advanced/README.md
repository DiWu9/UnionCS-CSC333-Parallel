<center>Week 4: Advanced MPI</center>

# Introduction

This week we learned advanced MPI collective communication, most importantly `MPI_Reduce`, one of the most important concepts in parallel programming.  We'll start by doing some in-class exercises, then move on to a fun application of the prefix sum algorithm.


# In Class Thursday

## Vector Normalization round 2

* Start by copying your vector normalization code into this directory
* update the code to use `MPI_Allreduce` with the `MPI_Sum` operator to get the sum of squares shared with *all* tasks, instead of having the root node receive, accumulate, and then send the sum of squares.
* time your code across several tasks and vector sizes.  Make a note in your `Writeup.md`

## Vector Mean

* Similarly, create an `MPI_Vector_Mean.c` and use the collective communication tools to calculate the mean of a vector. 
* Each task should create its *own* local array with random values, and then calculate the local sum.
* Task 0 should then use `MPI_Reduce` to accumulate the local sums.
* time your code across several tasks and vector sizes.  Make a note in your `Writeup.md`

## Vector STD

Now we'll implement standard deviation using the formula and method discussed in lecture

* Next create an `MPI_Vector_STD.c` Using the standard deviation formula.
* Each task should create its own random vector and calculate its local sum
* Each task should use `MPI_Allreduce` to obtain the global sum, and use that to calculate the global mean
* Each task should calculate its local sum of squared differences (squared distance between the value at every index and the global mean) 
* Task 0 should then Reduce these local sum squared differences to calculate the standard deviation.


## Submit:

* add/commit/push your solutions above
* Use tables in your `Writeup.md` to provide timing data that is nicely formatted
