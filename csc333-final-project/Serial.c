#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include <time.h>

// Serial version: Authored by Jingyu Yao May 29th
// Reference: https://www2.cs.siu.edu/~mengxia/Courses%20PPT/520/Chapter_6.ppt
// Reference: Pacheco. Chapter 6 nBody Solver

const double G = 6.673e-6;  //Constant g

typedef double vec_2d[2];
#define X 0    // all subscript for X
#define Y 1    // all subscript for Y

struct body {
    double m;
    vec_2d s;  // pos
    vec_2d v;  // v
};


void generate_initial_state(struct body bodies[], int n) {
    for (int i = 0; i < n; i++) {
        srand(time(NULL) + i);
        bodies[i].m = drand48() * 10;
        bodies[i].s[X] = drand48() * 20;
        bodies[i].s[Y] = drand48() * 20;
        bodies[i].v[X] = drand48() * 5;
        bodies[i].v[Y] = drand48() * 5;
    }
}

void read_initial_state(struct body bodies[], int n) {
    FILE *fp1;
    fp1 = fopen("initial.txt", "r");

    for (int i = 0; i < n; i++) {
        fscanf(fp1, "%lf", &bodies[i].m);
        fscanf(fp1, "%lf", &bodies[i].s[X]);
        fscanf(fp1, "%lf", &bodies[i].s[Y]);
        fscanf(fp1, "%lf", &bodies[i].v[X]);
        fscanf(fp1, "%lf", &bodies[i].v[Y]);
    }

    fclose(fp1);
}

void print_body_states(struct body bodies[], int n) {
    printf("mass    pos_x    pos_y   v_x    v_y\n");

    for (int i = 0; i < n; i++) {
        printf("Body %d: ", i);
        printf("%10.3e ", bodies[i].m);
        printf("%10.6f ", bodies[i].s[X]);
        printf("%10.6f ", bodies[i].s[Y]);
        printf("%10.6f ", bodies[i].v[X]);
        printf("%10.6f\n", bodies[i].v[Y]);
    }
    printf("\n");
}

void body_states_pos(struct body bodies[], int n_steps, int curr_step, int n, double pos[n_steps][n][2]) {
    for (int i=0; i<n; i++) {
        pos[curr_step][i][X] = bodies[i].s[X];
        pos[curr_step][i][Y] = bodies[i].s[Y];
    }
}

void print_pos(int n_steps, int n, double pos[n_steps][n][2]) {
    int i,j,k;
    for(i=0;i<n_steps;i++)
    {
        for(j=0;j<n;j++)
        {
            for(k=0;k<2;k++)
            {
                printf("%10.6f ",pos[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

void calculate_force(int curr_body, vec_2d forces[], struct body bodies[], int n) {
    int k;
    vec_2d force_qk;
    double dist;

    for (k = curr_body + 1; k < n; k++) {

        force_qk[X] = bodies[curr_body].s[X] - bodies[k].s[X];
        force_qk[Y] = bodies[curr_body].s[Y] - bodies[k].s[Y];
//        printf("%f ", force_qk[X]);
//        printf("%f ", force_qk[Y]);

        dist = sqrt(force_qk[X] * force_qk[X] + force_qk[Y] * force_qk[Y]);
        dist = pow(dist, 3);
        
        force_qk[X] *= ((-G * bodies[curr_body].m * bodies[k].m)/dist);
        force_qk[Y] *= ((-G * bodies[curr_body].m * bodies[k].m)/dist);
//        printf("%f ", force_qk[X]);
//        printf("%f ", force_qk[Y]);

        {
            forces[curr_body][X] += force_qk[X];
            forces[curr_body][Y] += force_qk[Y];
            forces[k][X] -= force_qk[X];
            forces[k][Y] -= force_qk[Y];
        }
    }
}

void calculate_momentum(struct body bodies[], int n){
    double mx = 0;
    double my = 0;
    for (int i=0; i<n; i++) {
        mx += (bodies[i].m * bodies[i].v[X]);
        my += (bodies[i].m * bodies[i].v[Y]);
    }
    printf("%f ", mx);
    printf("%f ", my);
    printf("\n");
}

void update_position(int curr_body, vec_2d forces[], struct body bodies[], double delta_t) {
//    printf("%f ", forces[curr_body][X]);
//    printf("%f ", forces[curr_body][Y]);

    bodies[curr_body].s[X] += delta_t * bodies[curr_body].v[X];
    bodies[curr_body].s[Y] += delta_t * bodies[curr_body].v[Y];
    bodies[curr_body].v[X] += delta_t * forces[curr_body][X] / bodies[curr_body].m;
    bodies[curr_body].v[Y] += delta_t * forces[curr_body][Y] / bodies[curr_body].m;
}

int main(int argc, char *argv[])
{
    int thread_count;
    int n;                      // numbodies
    int n_steps;                // numsteps
    int step;                   // curr_step
    int output_freq;            //print freq
    double delta_t;             // timestep size
    double time;                // time
    struct body * bodies;
    vec_2d* force;
    char init_flag;


    if (argc != 7) {
        printf("usage: nbody <thread count> <number of particles> <number of timesteps> <size of timestep> <output frequency> <1 or 0>\n");
        printf("1: generate bodies. 0: read bodies from initial.txt.\n");

        exit(0);
    } else {
        thread_count = atoi(argv[1]);
        n = atoi(argv[2]);
        n_steps = atoi(argv[3]);
        delta_t = atoi(argv[4]);
        output_freq = atoi(argv[5]);
        init_flag = atoi(argv[6]);
    }
    bodies = malloc(n * sizeof(struct body));
    force = calloc(n, sizeof(vec_2d));
    double pos[n_steps+1][n][2];

    if (init_flag == 1) {
        generate_initial_state(bodies, n);
    } else if (init_flag == 0) {
        read_initial_state(bodies, n);
    } else {
        printf("Wrong flag: 1: generate bodies, 0: read bodies from initial.txt");
        exit(0);
    }

    print_body_states(bodies, n);
    body_states_pos(bodies, n_steps+1, 0, n, pos);
    calculate_momentum(bodies, n);

    double start, end;
    start = omp_get_wtime();
    int i;

        for (step = 1; step <= n_steps; step++) {
            time = step * delta_t;

            for (i = 0; i < n-1; i++) {
                calculate_force(i, force, bodies, n);
            }

            for (i = 0; i < n; i++) {
                update_position(i, force, bodies, delta_t);
            }

            if (step % output_freq == 0) {

                printf("Current time: %.4f\n", time);

                print_body_states(bodies, n);
            }

            body_states_pos(bodies, n_steps+1, step, n, pos);
        }

    print_pos(n_steps+1, n, pos);
    calculate_momentum(bodies, n);

    end = omp_get_wtime();
    printf("Elapsed time = %e seconds\n", end-start);

    free(bodies);
    free(force);
    return 0;
}