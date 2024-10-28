#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size, local_sum, global_sum, step;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Initialize local_sum with the rank value
    local_sum = rank;

    // Tree-structured reduction with handling for any comm size
    for (step = 1; step < size; step *= 2)
    {
        if (rank % (2 * step) == 0)
        {
            int received_sum = 0;
            int source = rank + step;
            if (source < size)
            {
                MPI_Recv(&received_sum, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            local_sum += received_sum;
        }
        else if (rank % (2 * step) == step)
        {
            int dest = rank - step;
            MPI_Send(&local_sum, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
        }
    }

    // Handle any remaining processes
    if (rank == 0)
    {
        global_sum = local_sum;
        for (int i = 1; i < size; i++)
        {
            int partial_sum;
            MPI_Recv(&partial_sum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            global_sum += partial_sum;
        }
        printf("Global sum: %d\n", global_sum);
    }
    else
    {
        // Non-root processes send their local sum to root
        MPI_Send(&local_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
