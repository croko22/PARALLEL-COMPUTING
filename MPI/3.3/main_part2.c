#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size, local_sum, global_sum, step;

    MPI_Init(&argc, &argv);               // Inicializar MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtener el rango del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtener el tamaño del comunicador

    // Inicializar local_sum con el valor del rango
    local_sum = rank;

    // Reducción en estructura de árbol con manejo para cualquier tamaño de comunicador
    for (step = 1; step < size; step *= 2)
    {
        if (rank % (2 * step) == 0)
        {
            int received_sum = 0;
            int source = rank + step;
            if (source < size)
            {
                MPI_Recv(&received_sum, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Recibir suma parcial
            }
            local_sum += received_sum; // Sumar la suma recibida a la suma local
        }
        else if (rank % (2 * step) == step)
        {
            int dest = rank - step;
            MPI_Send(&local_sum, 1, MPI_INT, dest, 0, MPI_COMM_WORLD); // Enviar suma local al destino
        }
    }

    // Manejar cualquier proceso restante
    if (rank == 0)
    {
        global_sum = local_sum;
        for (int i = 1; i < size; i++)
        {
            int partial_sum;
            MPI_Recv(&partial_sum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Recibir suma parcial de otros procesos
            global_sum += partial_sum;                                                   // Sumar la suma parcial a la suma global
        }
        printf("Suma global: %d\n", global_sum); // Imprimir la suma global
    }
    else
    {
        // Los procesos no raíz envían su suma local a la raíz
        MPI_Send(&local_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); // Enviar suma local al proceso raíz
    }

    MPI_Finalize(); // Finalizar MPI
    return 0;
}