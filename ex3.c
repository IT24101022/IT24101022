#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ITERATIONS 10000000

int main(int argc, char** argv) {
    int rank, size;
    long long local_count = 0;
    long long global_count = 0;
    double pi_estimate;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Barrier(MPI_COMM_WORLD); // Synchronize before starting timer
    start_time = MPI_Wtime();

    long long iterations_per_process = NUM_ITERATIONS / size;
    srand(time(NULL) + rank);

    for (long long i = 0; i < iterations_per_process; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;

        if (x * x + y * y <= 1.0) {
            local_count++;
        }
    }

    MPI_Reduce(&local_count, &global_count, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    end_time = MPI_Wtime();

    if (rank == 0) {
        pi_estimate = 4.0 * (double)global_count / (double)NUM_ITERATIONS;
        printf("Estimated value of Pi after %d iterations: %f\n", NUM_ITERATIONS, pi_estimate);
        printf("Time taken: %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}
