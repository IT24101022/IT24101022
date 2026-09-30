#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_NUM 10000000

int main(int argc, char** argv) {
    int rank, size;
    long long local_sum = 0;
    long long global_sum = 0;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Barrier(MPI_COMM_WORLD); // Synchronize before starting timer
    start_time = MPI_Wtime();

    long long numbers_per_process = MAX_NUM / size;
    long long start_num = rank * numbers_per_process + 1;
    long long end_num = (rank == size - 1) ? MAX_NUM : start_num + numbers_per_process - 1;

    for (long long i = start_num; i <= end_num; i++) {
        local_sum += i;
    }

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Total sum from 1 to %d is: %lld\n", MAX_NUM, global_sum);
        long long expected_sum = ((long long)MAX_NUM * (MAX_NUM + 1)) / 2;
        printf("Expected sum: %lld\n", expected_sum);
        printf("Time taken: %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}
