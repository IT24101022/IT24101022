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

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long long iterations_per_process = NUM_ITERATIONS / size;
    srand(time(NULL) + rank);

    for (long long i = 0; i < iterations_per_process; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;

        if (x * x + y * y <= 1.0) {
            local_count++;
        }
    }

    // Allocate buffer for BSend on sending processes
    int buffer_size;
    void* buffer = NULL;
    if (rank != 0) {
        MPI_Pack_size(1, MPI_LONG_LONG_INT, MPI_COMM_WORLD, &buffer_size);
        buffer_size += MPI_BSEND_OVERHEAD;
        buffer = malloc(buffer_size);
        MPI_Buffer_attach(buffer, buffer_size);
    }

    if (rank == 0) {
        global_count = local_count;
        for (int i = 1; i < size; i++) {
            long long received_count;
            MPI_Status status;
            MPI_Recv(&received_count, 1, MPI_LONG_LONG_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            global_count += received_count;
        }
        pi_estimate = 4.0 * (double)global_count / (double)NUM_ITERATIONS;
        printf("Estimated value of Pi after %d iterations: %f\n", NUM_ITERATIONS, pi_estimate);
    } else {
        // Use Buffered Send
        MPI_Bsend(&local_count, 1, MPI_LONG_LONG_INT, 0, 0, MPI_COMM_WORLD);
    }

    if (rank != 0) {
        MPI_Buffer_detach(&buffer, &buffer_size);
        free(buffer);
    }

    MPI_Finalize();
    return 0;
}
