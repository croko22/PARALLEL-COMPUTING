#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size, local_sum, global_sum;

    MPI_Init(&argc, &argv);               // Inicializar MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtener el rango del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtener el tamaño del comunicador

    // Inicializar local_sum con el valor del rango
    local_sum = rank;

    // Reducción en estructura de árbol
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

    // El proceso raíz (rango 0) tiene la suma global final
    global_sum = local_sum;
    if (rank == 0)
    {
        printf("Suma global: %d\n", global_sum);
    }

    MPI_Finalize(); // Finalizar MPI
    return 0;
}