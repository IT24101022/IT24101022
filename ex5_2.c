#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int rank, size;
    int data_to_send = 12345;
    int received_data = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (rank == 0) {
            printf("Please run with at least 2 processes.\n");
        }
        MPI_Finalize();
        return 0;
    }

    // Allocate buffer for BSend
    int buffer_size;
    MPI_Pack_size(1, MPI_INT, MPI_COMM_WORLD, &buffer_size);
    buffer_size += MPI_BSEND_OVERHEAD;
    void* buffer = malloc(buffer_size);
    MPI_Buffer_attach(buffer, buffer_size);

    if (rank == 0) {
        printf("Process 0 sending data (%d) using BSend...\n", data_to_send);
        // Using Buffered Send (BSend)
        MPI_Bsend(&data_to_send, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Process 0 sent data successfully.\n");
    } else if (rank == 1) {
        // Process 1 receives data
        MPI_Recv(&received_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received data: %d\n", received_data);
    }

    // Detach and free the buffer
    MPI_Buffer_detach(&buffer, &buffer_size);
    free(buffer);

    MPI_Finalize();
    return 0;
}
