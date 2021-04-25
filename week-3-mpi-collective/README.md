# <center> Week 3 Assignments</center>

# Hands-On: Vector Normalization



## Setup: 

* pull the class repo
* copy `week-3-mpi-collective` into your own repo
* add/commit/push the contents

## Description 

Following a lecture on collective communication protocols and the vector normalization task, implement parallelized vector norm.

Let's first operate under the assumption that the size of the array is evenly divisible by the number of tasks (pick an even number).  

Recall that vector normalization is $\frac{\hat{u}}{||u||}$

Recall the algorithm is as follows:

1. Generate vector on root (make sure N%p == 0)
2. scatter vector  to all nodes
3. all nodes calculate local sum-of-squares
4. all nodes send local sum back to root, root receives
5. root accumulates local sums, takes square root
6. root broadcasts sqrt to nodes
7. all nodes normalize their local vector
8. all nodes gather back to root node

In a file called `MPI_Normalize_Vector.c`, implement the normalization function.  You may use your Week 2 vector programs as starter code.

* you should have separate arrays for the original and result vector
* you can re-use the same array for `local-sum` and `local-norm`

## Tests:

 you'll know it works if the magnitude of the normalized vector is 1.  The magnitude is calculated by taking the square root of the sum of squares of the vector.  

 Write a parallelized function to calculate the magnitude of a vector.

 ## Analysis:

* calculate speedup for a range of vector sizes and tasks.  Use MPI timing code.

 ## Submit:
 *  add/commit/push your code and a `Writeup.md`

 ## Challenge: non-even slices

 Learn about `MPI_Scatterv`, and implement a version of vector normalization that works even when $N%P != 0$

---
---


 # Parallelizing a 1-D Cellular Automaton

 ## Setup:

 * pull the class repo
 * copy the `week-3-mpi-collective/cell/`


## Description

We'll be parallelizing a cellular automaton.  Class will involve a short lecture on CAs, and a brief overview of the CA I've written for you.


## Step 0: Compiling and running

* You can compile the code by typing `make`.  This will generate an executable called `mpicell`.  To run `mpicell`, you need to pass in the rule:

`mpirun -np 2 ./mpicell 30`

will run across two nodes and on rule 30.
 
## Step 1: Scatter the World


Look where the starter code says 'STEP ONE', figure out how to scatter the global world to each of the nodes (assume N%p == 0).  Implement and test this before proceeding. 

Test and debug your code.

## Step 2: neighbor communication 


Next, where the example code says 'STEP TWO', use a neighbor-communication scheme to solve the problem of sending and receiving adjacent cell values from your neighbors.  There are three ways to do this:

*  classic Send/Receive to solve the problem
*  use non-blocking iSend/iRecv to solve the problem.
*  use (blocking) MPI_Sendrecv()

You will have come up with a scheme during an in-class breakout.


NOTE: be sure to correctly replace the last lines of Distribute(), which currently set the received (and returned) values to 0. 
implement and test before proceeding

Describe your communication scheme in your `Writeup.md`


## Step 3: gathering to node 0

next, where the example code says 'STEP THREE', find a way to gather the distributed local cells back to node 0 in order to print

NOTE: in the long term it might be advantageous to gather/and print the global world every N iterations, instead of gathering/printing every single iteration.  

## Step 4: Scaling up

Finally, add code to be able to change the worldsize, the maxiters, and the ruleset used via  command-line input.


Test your CA using several well known and easily recognizable CA rulesets.  

Measure speedup across several worldsizes and 

## Challenge:

Find a way to make the printouts of the cellworld look more interesting to look at, either by printing different characters to the screen, or by making a ppm or png file?

## Submit (By Tuesday Week 4)

* add/commit/push your code and Writeup




 
