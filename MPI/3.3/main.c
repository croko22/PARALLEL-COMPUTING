#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size, local_sum, global_sum;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Initialize local_sum with the rank value
    local_sum = rank;

    // Tree-structured reduction
    for (int step = 1; step < size; step *= 2)
    {
        if (rank % (2 * step) == 0)
        {
            int received_sum;
            MPI_Recv(&received_sum, 1, MPI_INT, rank + step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            local_sum += received_sum;
        }
        else if (rank % (2 * step) == step)
        {
            MPI_Send(&local_sum, 1, MPI_INT, rank - step, 0, MPI_COMM_WORLD);
        }
    }

    // The root process (rank 0) has the final global sum
    global_sum = local_sum;
    if (rank == 0)
    {
        printf("Global sum: %d\n", global_sum);
    }

    MPI_Finalize();
    return 0;
}
