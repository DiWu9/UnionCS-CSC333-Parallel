# <center> Final Project - N Body Solver </center>
## <center> Jasper, Jingyu, Woody </center>

# Problem Description

The N-body problem aims to find the positions and the velocities of a collection of particles over a period of time. We want to simulate the motion of the planets and the stars. The **input** of the program includes

- ``n``: number of particles
- ``delta_t``, ``T``: unit time interval and interval
- ``mass`` for each particle
- ``initial position`` for each particle
- ``initial velocity`` for each particle.

We will use Newton's second and third law of motion and the law of universal gravitation to determine the positions and velocities of stars for each interval ``delta_t``. For each time interval, we have to re-calculate the total force on each star ``q`` and get its acceleration, which is the second derivative of ``s_q``. Based on the acceleration, we can estimate the velocity and the position of the stars at the current timestep using Euler's method. The **output** of the problem is the position and the velocities of starts at time 0, delta_t, 2\*delta_t, ..., T\*delta_t.

# Pseudocode

The brute force reduction of the force on a particle is the sum of the forces exerted by the other particles. Hence for each particle `q`, we will compute the force exerted by all other particles `p`. The force exerted on `q` by `p` can be computed by the **universal gravitation formula** Fqk = - (G \* Mq \* Mp) \* (Sq \* Sp) / |Sq - Sp|^3.

We can simplify the force reduction algorithm by the fact that if there is a force exerted by `p` on `q` denoted as `Fpq`, then there is a same force in reverse direction exerted by `q` on `p`, `Fqp = -Fpq`. Hence to reduce the force on particle `q`, we only need to calculate the forces exerted by the partiles that is after the particle `q` because we have already known the forces exerted by particles before `q` that is on `q`. Hence we have the pseudocode below (Pacheco. P274).

    for each particle q forces[q] = 0;
    for each particle q {
        for each particle k > q {
            x_diff = pos[q][X] − pos[k][X];
            y_diff = pos[q][Y] − pos[k][Y];
            dist = sqrt(x_diff∗x_diff + y_diff∗y_diff);
            dist_cubed = dist∗dist∗dist;
            force_qk[X] = G∗masses[q]∗masses[k]/dist_cubed ∗ x_diff; 
            force_qk[Y] = G∗masses[q]∗masses[k]/dist_cubed ∗ y_diff;
            forces[q][X] += force_qk[X]; 
            forces[q][Y] += force_qk[Y]; 
            forces[k][X] −= force_qk[X]; 
            forces[k][Y] −= force_qk[Y];
    } }

For the last four lines in the nested for loop, we add the force exerted by particle `k` on `q` on both X and Y direction to the total force of `q`. Also add the counterforce of `q` on `k` to the total force of `k`. Hence when the outer loop reaches `k`, the inner loop can save the time to re-calculate the `force_kq`.


# Serial Implementation

The serial version of our nbody solver established the basic skeleton of our program and offers
a working linear solution to the nbody problem.

Our serial solver takes in 6 arguments in addition to the executable, which are:

    <thread count> <number of particles> <number of timesteps> <size of timestep> <output frequency> <chunksize> <initiate flag>

The initiate flag takes values of only 1 or 0, with 1: generate bodies; 0: read bodies from initial.txt file.

Using these parameters we can explain the structure of our serial nbody solver. The thread_count
argument is a placeholder and does not have actual effects in our serial solver. We take in the number of particles,
number of timesteps, size of timestep, output frequency, and an initiate flag. Number of particles,
number of timesteps are pretty self-explanatory, while size of timestep will serve as the delta_t given in the
textbook(this argument is not actually mandatory as we can assume a delta of 1 or 0.1, but we have given the choice of altering here).

Key structures/variables are listed below and their usage/purpose explained:

    typedef double vec_2d[2];   // definition of a 2d vector

    struct body {
    // key struct used to define everything about a body
    double m;  // mass
    vec_2d s;  // position
    vec_2d v;  // velocity
    };

    int thread_count;       // placeholder thread_count, not useful in serial
    int n;                  // numbodies
    int n_steps;            // numsteps
    int step;               // iterator for timesteps
    int output_freq;        // print frequency
    double delta_t;         // timestep size
    double time;            // current time, usually equals to step*delta_t
    struct body * bodies;   // array of bodies of size numbodies, records everything about the list of bodies we have
    vec_2d* force;          // array of 2d array of size numbodies, records the force acting on each body respectively
    char init_flag;         // initiate flag, 0 or 1

Key functions are listed below and their usage/purpose explained:

    void generate_initial_state(struct body bodies[], int n)    // generate a list of random bodies of size n
    void read_initial_state(struct body bodies[], int n)        // read a list of bodies from initial.txt
    void print_body_states(struct body bodies[], int n)         // given the list of bodies, print out their mass, position, velocity
    void calculate_momentum(struct body bodies[], int n)        // KEY TESTING FUNCTION used to calculate total momentum of the system. The system momentum should be exactly the same across all timesteps. Removed after ensuring correctness.

    // IMPORTANT: key functions that implement gravitational computations as specified in Problem Description
    void calculate_force(int curr_body, vec_2d forces[], struct body bodies[], int n)           //KEY CALCULATION FUNCTION given a current body, and the list of bodies with their current states, compute the force this body exert on all others and modify forces[] accordingly
    void update_position(int curr_body, vec_2d forces[], struct body bodies[], double delta_t)  //KEY CALCULATION FUNCTION given a current body, calculate its new position according to the force exerted on it via forces[]

Main logic:

    get initialized information
    for each timestep:
        compute forces[] for the current timestep
        update bodies[] using forces[]
    print bodies/verify correctness if needed

# Parallel Implementation

## MPI

## OPENMP - Jingyu

This version of OMP parallelization is based on the serial version. The main parallel
objective is the timestep loop, which includes two nested loops for force calculation and position update.
The outer timestep loop is serial, so I have chosen to parallelize each inner loop individually.

Since our force calculation and position update are each written inside their own function, it seems like we can just
call two simple omp parallel for and let OMP take care of distributing the tasks. This is true for update_position(), but not
so much for calculate_force(). Within calculate_force():

    forces[curr_body][X] += force_qk[X];
    forces[curr_body][Y] += force_qk[Y];
    forces[k][X] -= force_qk[X];
    forces[k][Y] -= force_qk[Y];

These lines update the forces exerted on each body according to the reduced algorithm given in the textbook
and must be executed serially, otherwise a race condition will exist. This will result in: 1. incorrect cumulation of forces; 2. incorrect calculation of respective forces pairs.
Note, the entire block must be executed serially thus we cannot use directives like `omp atomic`.

In order to avoid using a omp critical block, we can solve this by exchanging space for speed. Instead of using
one `vec_2d* force` array of size `numbodies`, we can create a 2d array `vec_2d* force` of size `nthreads * numbodies`.
In essence, we have 2d array `force[nthreads][numbodies]`. Each thread will compute the forces that exert on all other bodies by the bodies that thread is responsible for, and then store
those forces only in their local force array. After all computations are finished, we will collect all these calculated forces and reduce them into our
`vec_2d* force` array, which also can be done in parallel.

According to the textbook, a cyclic scheduling yields better perform as the task division is more equal
among the threads. Experimentation shows that a chunksize of 4 yields the best speedup. To sum up, my
parallel version alters the serial version as follows:

    for each timestep:     
        # pragma omp for schedule(static, chunksize)    
        for each body:              
            calculate_force();
            // calculate force with cyclic schedule, the given force array pointer to calculate_force() points to the start of each subarray

        # pragma omp for schedule(static, chunksize)
        for each body:                
            for each thread:
                reduce the results of forces from all subarrays into forces[]

        # pragma omp for schedule(static, chunksize)
        for each body:                
            update_position();

## OPENMP - Woody

Initially, I implemented the basic parallel version using OPENMP by adding `# pragma omp parall for` clause on top of two for loops. The first for loop reduce the force on each particle `p`, and the second for loop update the velocity and the position of each particle `p` based on the updated total force.

    for (step = 1; step <= n_steps; step++)
        {
            time = step * delta_t;

            # pragma omp parallel for schedule(static, 8) num_threads(thread_count)
            for (i = 0; i < n - 1; i++)
            {
                calculate_force(i, force, bodies, n);
            }

            # pragma omp parallel for schedule(static, 8) num_threads(thread_count)
            for (i = 0; i < n; i++)
            {
                update_position(i, force, bodies, n, delta_t);
            }
    }

By reading the instruction on the textbook (Pacheco. 281), I noticed that forking and joining of threads is something that should be improved. The forking suggests the time that the `pragma` directive branch the master thread into parallel threads, and the joining means the parallel threads are converging back to be the master thread. The forking and joining can be eliminated by putting the `pragma omp parallel` at the outer loop (timestep loop). Hence contents inside the timestep loop are running by parallel threads. Thus we need to put `# pragma omp single` before the lines that should only be executed once.

The race condition exists while summing the force because multiple threads can simultaneouly changing the total force of particle `k`. For example, both the thread summing forces on particle `p` and the thread summing the forces on particle `p+1` will modify the total force of `p+2`. Hence the total force of `p+2` may be incorrect if both threads are writting on it simultaneously. To address the race condition, I implemented locks suggested on page 286. 

    omp_set_lock(&locks[curr_body]);
    forces[curr_body][X] += force_qk[X];
    forces[curr_body][Y] += force_qk[Y];
    omp_unset_lock(&locks[curr_body]);

    omp_set_lock(&locks[k]);
    forces[k][X] -= force_qk[X];
    forces[k][Y] -= force_qk[Y];
    omp_unset_lock(&locks[k]);

As suggested by the code block above, when modifying the particle `curr_body` and `k`, the particle `curr_body` and `k` is locked and thus cannot be modified by other threads. However, other particles are allowed to be modifed when `curr_body` and `k` are locked. Hence the lock solution is theoratically faster than solving the problem using critical clause.


# Speedup Analysis

## MPI
1000 timesteps, timestep = 1.

Ran on SLURM cluster.

Efficiency in parentheses

| n\particles | 64          | 128          | 256       | 512          |
|-------------|-------------|--------------|-----------|--------------|
| 1           | 416ms       | 1522ms       | 6201ms    | 24968ms      |
| 2           | 341ms (61%) | 1039ms (73%) | 3440(90%) | 12764ms(97%) |
| 4           | 327ms (31%) | 804ms (47%)  | 2099(73%) | 7123ms(87%)  |
| 8           | 375ms (14%) | 615ms (30%)  | 1404(55%) | 4228ms(73%)  |


## Analysis

## OPENMP - Jingyu
Ran on skylake computer.
1000 timesteps, timestep = 1, chunksize = 4

All outputs disabled(output_freq = 9999).

| n\particles | 64       | 128       | 256      | 512       |
|-------------|----------|-----------|----------|-----------|
| 1           | 60ms     | 174ms     | 638ms    | 2479ms    |
| 2           | 43ms (70%) | 134ms (65%) | 423ms(75%) | 1846ms(67%) |
| 4           | 42ms (35%) | 113ms (38%) | 319ms(50%) | 1031ms(60%) |
| 8           | 47ms (16%) | 103ms (21%) | 289ms(28%) | 862ms(36%)  |
| 16          | 46ms (8.1%) | 99ms (11%)  | 266ms(15%) | 782ms(20%)  |
| 32          | 53ms (3.5%) | 103ms (5.3%) | 211ms(9.5%) | 618ms(12%)  |

## Analysis
In general, computation time always decrease as the number of threads increase, meaning that my
program immediately receive speedup from parallelization. This is greatly due to eliminating any serial portions of the program
so IO takes much less dominance.

In general, efficiency decreases as the number of threads increase, but increases as the number of bodies increase.
As the number of threads increase, IO becomes more and more dominant over the program thus the efficiency will surely decrease. However, if we increase
the size of the task, then we are indirectly decreasing the % that IO takes in the program, thus leading to a higher efficiency.

For smaller task sizes, such as 64-128 bodies, I stop seeing noticeable speedup as approximately when going from 4 threads to 8 threads, meaning that
at this point additional IO would bring less benefit than the overhead it comes with. For larger task sizes, the speedup stop generally happens when going from
16 threads to 32 threads.

## OPENMP - Woody
1000 timesteps, timestep = 1
- Ran on skylake computer:

| n\particles | 64           | 128            | 256         | 512            |
|-------------|--------------|----------------|-------------|----------------|
| 1           | 104ms        | 356ms          | 1403ms      | 5408ms         |
| 2           | 468ms (11%)  | 1644ms (10.8%) | 4945(14.2%) | 13156ms(20.5%) |
| 4           | 463ms (5.6%) | 1283ms (6.9%)  | 3904(9%)    | 7989ms(16.9%)  |
| 8           | 402ms (3.2%) | 1240ms (3.6%)  | 3374(5.1%)  | 7134ms(9.4%)   |
| 16          | 385ms (1.7%) | 1087ms (2%)    | 3118(2.8%)  | 6798ms(4.9%)   |
| 32          | 383ms (0.8%) | 1005ms (1.1%)  | 2728(1.6%)  | 6963ms(2.4%)   |


- Ran on Macbook-Pro

| n\particles | 64            | 128          | 256           | 512           |
|-------------|---------------|--------------|---------------|---------------|
| 1           | 138ms         | 559ms        | 2203ms        | 8766ms        |
| 2           | 176ms (39.2%) | 608ms (50%)  | 2071ms(53%)   | 6742ms(65.8%) |
| 4           | 117ms (29.4%) | 398ms (35%)  | 1394ms(39.5%) | 5074ms(43.2%) |
| 8           | 89ms (19.3%)  | 277ms (25%)  | 1213ms(22.7%) | 3319ms(33%)   |
| 16          | 336ms (2.6%)  | 736ms (4.7%) | 1748ms(9.3%)  | 4754ms(22.5%) |

## Analysis

Based on the table collected on Mac, we can observe that for any number of particles, 2-thread program has the maximum efficiency among non-serial programs, and 8-thread program has the minimum execution time among both serial and parallel programs. If there is an efficiency on serial prorgam, it should be 1. Then we can see that the efficiency decreases while the number of threads increase, suggesting that when the number of threads increases the average speed of each thread decreases. This is because it takes extra time for the threads to cooperate to finish the work. Extra time may be comsumed on avoiding race condition, and non-parallelize codes. In terms of execition time, the execition time keeps decreasing until the number of threads reaches 8. The rule apply for any task size. I assume this could be casused by the locks becuase if there are too many threads, a lot of particles will be locked and thus other threads accessing the same particles have to wait until the partile is unlocked.

It is interesting to observe that the same program tested on skylake produces a totally different result. The serial program becomes faster on skylake than on mac when computing the same number of particles. However, the execution time of the parallel version significantly increasees when processing the same number of particles.

# Wattage

With this simulation, it is extremely easy to efficiently use a large amount of computing time, especially when the number of particles is large.  Other projects we did in class like vector and matrix arithmetic, weren't so easy to get a large wattage because in order for there to be a lot of compute time either the input size or the number of iterations has to be very large. However, this is not the case for the N-body problem because the compute time scale quadratically with the number of particles, allowing for a relatively small amount of particles, say 10,000 which doesn't take up that much ram or parallel communication overhead to need a large amount of computing time. In the case of 10,000 particles, it would take approximately (10,000)^2/2 force computations per timestep.

# Utility

Nbody solvers is a classic parallel program and could offers significant space for parallel optimization. Going from this collection of 2d solvers to a future class project is extremely easy, and will also save you a lot of time as we ourselves have implemented several versions of parallelization, with each using different aspects of OpenMP and MPI.
Our timings also show that our different solutions to parallelization yields different levels of timings showing that certain solutions might offer better performance than others. For example, although using locks is one way to eliminate critical omp blocks, it still cannot compete with the performance of using subarrays in which serial
code portions are eliminated entirely. This shows how much dominance IO has as a single serial portion of the program can significantly slow things down.

A core utility of doing the project was directly being able to compare MPI and OpenMP given the same initial serial implementation. One thing that is clear from looking at the code of the implementation is that OpenMP code, is not too dissimilar from the original, This is mainly because all of the threads have access to the same memory. Whereas in MPI you have to be explicit on what core gets what chunk of memory and such. Another thing is that OpenMP seems to run initially faster than MPI, due to either a better implementation or that OpenMP has less overhead than MPI. 

Another Utility of this is to model any particle-particle interaction, not just planets. The exact same program, with some variable changing, could be used to model the interactions and movements between charged particles and any other interaction that has force proportional to the properties of the two particles.

Finally, going from a 2d solver to a 3d solver really isn't a much harder task using this skeleton, as we only need to update calculate_force() and update_position, with other minor tweaks of using 3d vectors instead of 2d.