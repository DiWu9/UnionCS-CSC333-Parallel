#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>
#include <time.h>

// MPI: parallel version, Jasper Bergh

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
    printf("          mass        pos_x      pos_y      v_x       v_y\n");

    for (int i = 0; i < n; i++) {
        printf("Body %2d: ", i);
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

void calculate_force(int curr_body, struct body bodies[], vec_2d my_forces[], int n, int my_start) {
    int k;
    vec_2d force_qk;
    double dist;
    my_forces[curr_body - my_start][X] = 0;
    my_forces[curr_body - my_start][Y] = 0;
    for (k = 0; k < n; k++) {
        if(k != curr_body){
            force_qk[X] = bodies[curr_body].s[X] - bodies[k].s[X];
            force_qk[Y] = bodies[curr_body].s[Y] - bodies[k].s[Y];
        //    printf("%f ", force_qk[X]);
        //    printf("%f ", force_qk[Y]);

            dist = sqrt(force_qk[X] * force_qk[X] + force_qk[Y] * force_qk[Y]);
            dist = pow(dist, 3);
        //    printf("%.10f ", fact);
        //    printf("%.10f ", fact);
            force_qk[X] *= ((-G * bodies[curr_body].m * bodies[k].m)/dist);
            force_qk[Y] *= ((-G * bodies[curr_body].m * bodies[k].m)/dist);
        //    printf("%f ", force_qk[X]);
        //    printf("%f \n", force_qk[Y]);

            my_forces[curr_body - my_start][X] += force_qk[X];
            my_forces[curr_body - my_start][Y] += force_qk[Y];
        }
    }
}

void print_momentum(struct body bodies[], int n){
    double mx = 0;
    double my = 0;
    for (int i=0; i<n; i++) {
        mx += (bodies[i].m * bodies[i].v[X]);
        my += (bodies[i].m * bodies[i].v[Y]);
    }
    printf("Total Momentum: %f ", mx);
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

void update_local(int my_rank, struct body bodies[], struct body my_bodies[], vec_2d my_forces[],\
                  int my_start, int my_end, int n, double delta_t){
    int i;
    int my_size = my_end - my_start;
    for(i = 0; i < my_size; i++){ //Update Forces
        calculate_force(i, bodies, my_forces, n, my_start);
    }

    for(i = 0; i< my_size; i++){
        update_position(i, my_forces, my_bodies , delta_t);
    }
}

int main(int argc, char *argv[])
{
	int        num_procs;               /* Number of processes    */
	int        my_rank;               /* My process rank        */
	//Don't put any MPI stuff before this line
	MPI_Init(&argc, &argv);  //sets up the MPI. Always this line!
	/* Get the number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	/* Get my rank among all the processes */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	MPI_Datatype MPI_BODY_SIZE;
	MPI_Type_contiguous(5, MPI_DOUBLE, &MPI_BODY_SIZE);
	MPI_Type_commit(&MPI_BODY_SIZE);

    MPI_Datatype MPI_FORCE_SIZE;
	MPI_Type_contiguous(2, MPI_DOUBLE, &MPI_FORCE_SIZE);
	MPI_Type_commit(&MPI_FORCE_SIZE);
		// do some stuff

	int thread_count;
    int n;                      // numbodies
    int n_steps;                // numsteps
    int step;                   // curr_step
    int output_freq;            //print freq
    double delta_t;             // timestep size
    double elapsed_time;                // time
    struct body * bodies;
    //vec_2d* force;
	struct body * my_bodies;
    vec_2d* my_forces;
    char init_flag;

	srand48(time(NULL) + my_rank);

	if (argc != 6) {
		if(my_rank == 0){
			printf("usage: nbody <number of particles> <number of timesteps> <size of timestep> <output frequency> <1 or 0>\n");
			printf("1: generate bodies. 0: read bodies from initial.txt.\n");
		}
		MPI_Finalize();
		exit(0);	
	}
	thread_count = num_procs;
	n = atoi(argv[1]);
	n_steps = atoi(argv[2]);
	delta_t = atoi(argv[3]);
	output_freq = atoi(argv[4]);
	init_flag = atoi(argv[5]);

	if(n % thread_count != 0){
		if(my_rank == 0)
			printf("Number of particles must be divisible by Number of threads\n");
		MPI_Finalize();
		exit(0);	
	}

	if (my_rank == 0 && init_flag != 0 && init_flag != 1){
		if(my_rank == 0)
		printf("Wrong flag: 1: generate bodies, 0: read bodies from initial.txt\n");
		MPI_Finalize();
		exit(0);
	}


	bodies = malloc(n * sizeof(struct body));
    //force = calloc(n, sizeof(vec_2d));
	if(my_rank == 0){
		if (init_flag == 1) {
			generate_initial_state(bodies, n);
		} else if (init_flag == 0) {
			read_initial_state(bodies, n);
		}
	}
	// Broadcast inital bodies to all
	MPI_Bcast(bodies, n, MPI_BODY_SIZE, 0, MPI_COMM_WORLD);
	

	int my_count = n/thread_count;
	int my_start = floor(my_rank * n / thread_count);
	int my_end = floor((my_rank + 1) * n / thread_count);
	//double my_pos[n_steps+1][my_count][2];
	
    
	
	my_bodies = malloc(my_count * sizeof(struct body));
    my_forces = malloc(my_count * sizeof(vec_2d));
	MPI_Scatter(bodies, my_count, MPI_BODY_SIZE, my_bodies, my_count, MPI_BODY_SIZE ,0, MPI_COMM_WORLD);
    // double pos[n_steps+1][n][2];

    
    //print_body_states(my_bodies, my_count);
    //body_states_pos(bodies + my_start, n_steps+1, 0, my_count, my_pos);
    int i;
    double startTime;
    if(my_rank == 0){
        printf("Step #0\n");
        print_body_states(bodies, n);
        print_momentum(bodies, n);
        startTime = MPI_Wtime();
    }
    for (step = 1; step <= n_steps; step++) {
        for(i = 0; i < my_count; i++){ //Update Forces
            calculate_force(i + my_start, bodies, my_forces, n, my_start);
        }

        for(i = 0; i< my_count; i++){
            update_position(i, my_forces, my_bodies , delta_t);
        }

        MPI_Allgather(my_bodies, my_count, MPI_BODY_SIZE, bodies, my_count, MPI_BODY_SIZE, MPI_COMM_WORLD);

    }
    if(my_rank == 0){
        double endTime = MPI_Wtime();
        double elapsed =  (double)(endTime - startTime) * 1000.0;
        printf("Step #%d\n",step);
        print_body_states(bodies, n);
        print_momentum(bodies, n);
        printf("\nthat took %f ms\n",elapsed);
    }
    
    MPI_Finalize();
	//please no MPI stuff after this line
    return 0;
}