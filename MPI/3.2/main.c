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
        // Read the total number of tosses
        printf("Enter the total number of tosses: ");
        scanf("%lld", &tosses);
    }

    // Broadcast the number of tosses to all processes
    MPI_Bcast(&tosses, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

    // Divide tosses among processes
    local_tosses = tosses / size;

    // Seed the random number generator uniquely for each process
    srand(time(NULL) + rank);

    // Monte Carlo simulation: Count points inside the circle
    for (long long int toss = 0; toss < local_tosses; toss++)
    {
        x = (double)rand() / RAND_MAX * 2.0 - 1.0;
        y = (double)rand() / RAND_MAX * 2.0 - 1.0;
        if (x * x + y * y <= 1.0)
        {
            number_in_circle++;
        }
    }

    // Use MPI_Reduce to collect the sum of points inside the circle
    MPI_Reduce(&number_in_circle, &global_in_circle, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        // Estimate π
        pi_estimate = 4.0 * (double)global_in_circle / (double)tosses;
        printf("Estimated value of π: %f\n", pi_estimate);
    }

    MPI_Finalize();
    return 0;
}
