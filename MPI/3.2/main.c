#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
    int rank, size;
    long long int tosses, local_tosses, number_in_circle = 0, global_in_circle;
    double x, y, pi_estimate;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        printf("Enter the total number of tosses: ");
        scanf("%lld", &tosses);
    }

    // Difundir el número de lanzamientos a todos los procesos
    MPI_Bcast(&tosses, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

    // Dividir los lanzamientos entre los procesos
    local_tosses = tosses / size;

    // Sembrar el generador de números aleatorios de manera única para cada proceso
    srand(time(NULL) + rank);

    // Simulación de Monte Carlo: Contar puntos dentro del círculo
    for (long long int toss = 0; toss < local_tosses; toss++)
    {
        x = (double)rand() / RAND_MAX * 2.0 - 1.0;
        y = (double)rand() / RAND_MAX * 2.0 - 1.0;
        if (x * x + y * y <= 1.0)
        {
            number_in_circle++;
        }
    }

    // Usar MPI_Reduce para recolectar la suma de puntos dentro del círculo
    MPI_Reduce(&number_in_circle, &global_in_circle, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        // Estimar π
        pi_estimate = 4.0 * (double)global_in_circle / (double)tosses;
        printf("Valor estimado de π: %f\n", pi_estimate);
    }

    MPI_Finalize();
    return 0;
}
