#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    int data = 100;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 3) {
        if (rank == 0) {
            printf("Please run with at least 3 processes for this exercise.\n");
        }
        MPI_Finalize();
        return 0;
    }

    if (rank == 0) {
        // Process 0 sends data to Process 1
        printf("Process 0 sending data to Process 1...\n");
        MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Process 0 sent data successfully.\n");
    } else if (rank == 1) {
        // Process 1 intentionally expects data from Process 2 instead of Process 0
        // This will cause a mismatch and the program will hang (Deadlock)
        printf("Process 1 waiting to receive data from Process 2...\n");
        MPI_Recv(&data, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received data: %d\n", data);
    } else if (rank == 2) {
        // Process 2 does nothing
        printf("Process 2 is idle.\n");
    }

    MPI_Finalize();
    return 0;
}
