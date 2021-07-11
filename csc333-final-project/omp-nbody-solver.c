#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include <time.h>

/*
author: Di Wu
date: 06/04/2021
description: parallel implementation of N-body solver using OpenMP
(p.s.) 
 - deal with forking and joining of threads by adding the parallel directive before the outmost loop
 - deal with race condition of the force reduction by locks

reference:  Pacheco. P.246-247 (locks) P.281-288 (nbody-omp)
compile: gcc -o omp-nbody-solver-lock -fopenmp omp-nbody-solver-lock.c -lm (linux)
*/

const double G = 6.673e-6; //Constant g

typedef double vec_2d[2];
#define X 0 // all subscript for X
#define Y 1 // all subscript for Y

struct body
{
    double m;
    vec_2d s; // pos
    vec_2d v; // v
};

void generate_initial_state(struct body bodies[], int n)
{
    for (int i = 0; i < n; i++)
    {
        srand(time(NULL) + i);
        bodies[i].m = drand48() * 10;
        bodies[i].s[X] = drand48() * 20;
        bodies[i].s[Y] = drand48() * 20;
        bodies[i].v[X] = drand48() * 5;
        bodies[i].v[Y] = drand48() * 5;
    }
}

void read_initial_state(struct body bodies[], int n)
{
    FILE *fp1;
    fp1 = fopen("initial.txt", "r");

    for (int i = 0; i < n; i++)
    {
        fscanf(fp1, "%lf", &bodies[i].m);
        fscanf(fp1, "%lf", &bodies[i].s[X]);
        fscanf(fp1, "%lf", &bodies[i].s[Y]);
        fscanf(fp1, "%lf", &bodies[i].v[X]);
        fscanf(fp1, "%lf", &bodies[i].v[Y]);
    }

    fclose(fp1);
}

void print_body_states(struct body bodies[], int n, double time)
{
    printf("Current time: .2%f\n", time);
    printf("         mass        pos_x      pos_y      v_x       v_y\n");

    for (int i = 0; i < n; i++)
    {
        printf("Body %d: ", i);
        printf("%10.3e ", bodies[i].m);
        printf("%10.6f ", bodies[i].s[X]);
        printf("%10.6f ", bodies[i].s[Y]);
        printf("%10.6f ", bodies[i].v[X]);
        printf("%10.6f\n", bodies[i].v[Y]);
    }
    printf("\n");
}

void body_states_pos(struct body bodies[], int n_steps, int curr_step, int n, double pos[n_steps][n][2])
{
    for (int i = 0; i < n; i++)
    {
        pos[curr_step][i][X] = bodies[i].s[X];
        pos[curr_step][i][Y] = bodies[i].s[Y];
    }
}

void print_pos(int n_steps, int n, double pos[n_steps][n][2])
{
    int i, j, k;
    for (i = 0; i < n_steps; i++)
    {
        for (j = 0; j < n; j++)
        {
            for (k = 0; k < 2; k++)
            {
                printf("%10.6f ", pos[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

void calculate_force(int curr_body, vec_2d forces[], struct body bodies[], int n, int chunk_size, omp_lock_t locks[])
{
    int k;
    vec_2d force_qk;
    double dist;

    for (k = curr_body + 1; k < n; k++)
    {

        force_qk[X] = bodies[curr_body].s[X] - bodies[k].s[X];
        force_qk[Y] = bodies[curr_body].s[Y] - bodies[k].s[Y];

        dist = sqrt(force_qk[X] * force_qk[X] + force_qk[Y] * force_qk[Y]);
        dist = dist * dist * dist;

        force_qk[X] *= ((-G * bodies[curr_body].m * bodies[k].m) / dist);
        force_qk[Y] *= ((-G * bodies[curr_body].m * bodies[k].m) / dist);

        // the chunk below may have race condition
        omp_set_lock(&locks[curr_body]);
        forces[curr_body][X] += force_qk[X];
        forces[curr_body][Y] += force_qk[Y];
        omp_unset_lock(&locks[curr_body]);

        omp_set_lock(&locks[k]);
        forces[k][X] -= force_qk[X];
        forces[k][Y] -= force_qk[Y];
        omp_unset_lock(&locks[k]);
    }
}

void calculate_momentum(struct body bodies[], int n, int curr_step)
{
    double mx = 0;
    double my = 0;
    for (int i = 0; i < n; i++)
    {
        mx += (bodies[i].m * bodies[i].v[X]);
        my += (bodies[i].m * bodies[i].v[Y]);
    }
    printf("Momentum at step %d: \n", curr_step);
    printf("%f ", mx);
    printf("%f \n", my);
    printf("\n");
}

void update_position(int curr_body, vec_2d forces[], struct body bodies[], double delta_t)
{
    /*
    printf("forces before computing velocity: \n");
    printf("%f ", forces[curr_body][X]);
    printf("%f \n", forces[curr_body][Y]);
    */

    bodies[curr_body].s[X] += delta_t * bodies[curr_body].v[X];
    bodies[curr_body].s[Y] += delta_t * bodies[curr_body].v[Y];
    bodies[curr_body].v[X] += delta_t * forces[curr_body][X] / bodies[curr_body].m;
    bodies[curr_body].v[Y] += delta_t * forces[curr_body][Y] / bodies[curr_body].m;
}

int main(int argc, char *argv[])
{
    int thread_count;
    int chunk_size;
    int n;           // numbodies
    int n_steps;     // numsteps
    int step;        // curr_step
    int output_freq; // print freq
    double delta_t;  // timestep size
    double time;     // time
    struct body *bodies;
    vec_2d *force;
    char init_flag;
    omp_lock_t *locks;

    if (argc != 8)
    {
        printf("usage: nbody <thread count> <number of particles> <number of timesteps> <size of timestep> <output frequency> <chunksize> <1 or 0>\n");
        printf("1: generate bodies. 0: read bodies from initial.txt.\n");

        exit(0);
    }
    else
    {
        thread_count = atoi(argv[1]); /* number of threads */
        n = atoi(argv[2]);            /* number of particles */
        n_steps = atoi(argv[3]);      /* number of timesteps */
        delta_t = atof(argv[4]);      /* size of timestep */
        output_freq = atoi(argv[5]);  /* output freq */
        chunk_size = atoi(argv[6]);   /* chunk size */
        init_flag = atoi(argv[7]);    /* 0 (read) / 1 (randomly generate) */
    }

    /*
    printf("Settings: \n");
    printf("number threads: %d\n", thread_count);
    printf("number particles: %d\n", n);
    printf("number timesteps: %d\n", n_steps);
    printf("delta t: %f\n", delta_t);
    printf("output frequency: %d\n", output_freq);
    printf("chunk size: %d\n", chunk_size);
    printf("input flag: %d\n", init_flag);
    */

    bodies = malloc(n * sizeof(struct body));
    force = calloc(n, sizeof(vec_2d));
    double pos[n_steps + 1][n][2]; /* records positions from step 1 to n */
    locks = malloc(n * sizeof(omp_lock_t));
    for (int i = 0; i < n; i++)
    {
        omp_init_lock(&locks[i]);
    }

    if (init_flag != 0 && init_flag != 1)
    {
        printf("Wrong flag: 1: generate bodies, 0: read bodies from initial.txt");
    }

    if (init_flag == 1)
    {
        generate_initial_state(bodies, n);
    }
    else if (init_flag == 0)
    {
        read_initial_state(bodies, n);
    }

    // print_body_states(bodies, n);
    body_states_pos(bodies, n_steps + 1, 0, n, pos);
    //calculate_momentum(bodies, n, 0);

    double start, end;
    start = omp_get_wtime();
    int i;

    //printf("number of steps: %d\n", n_steps);
    //printf("output freq: %d\n", output_freq);

#   pragma omp parallel num_threads(thread_count) default(none) \
    shared(bodies, force, delta_t, n_steps, n, pos, output_freq, chunk_size, locks) private(step, i, time)
    for (step = 1; step <= n_steps; step++)
    {
#   pragma omp single
        time = step * delta_t;
        if (step % output_freq == 0)
        {
#           pragma omp single
            print_body_states(bodies, n, time);
#           pragma omp single
            calculate_momentum(bodies, n, step); // err checking: momentum is the same
        }

#   pragma omp for schedule(static, chunk_size)
        for (i = 0; i < n - 1; i++)
        {
            calculate_force(i, force, bodies, n, chunk_size, locks);
        }

#   pragma omp for schedule(static, chunk_size)
        for (i = 0; i < n; i++)
        {
            update_position(i, force, bodies, delta_t);
        }

        //#       pragma omp single
        //body_states_pos(bodies, n_steps + 1, step, n, pos);
    }

    //print_pos(n_steps + 1, n, pos);

    end = omp_get_wtime();
    printf("Elapsed time = %.2f ms\n", (end - start) * 1000);

    for (int i = 0; i < n; i++)
    {
        omp_destroy_lock(&locks[i]);
    }
    free(bodies);
    free(force);
    free(locks);
    return 0;
}