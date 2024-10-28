#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void matrix_vector_multiply(double *local_matrix, double *vector, double *local_result, int local_n, int n)
{
    for (int i = 0; i < local_n; i++)
    {
        local_result[i] = 0.0;
        for (int j = 0; j < n; j++)
        {
            local_result[i] += local_matrix[i * n + j] * vector[j];
        }
    }
}

int main(int argc, char **argv)
{
    int rank, size, n;
    double *matrix = NULL, *vector = NULL, *local_matrix = NULL, *local_result = NULL, *result = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Assuming n is defined and is divisible by size
    if (rank == 0)
    {
        printf("Enter the order of the matrix (n): ");
        scanf("%d", &n);
        matrix = (double *)malloc(n * n * sizeof(double));
        vector = (double *)malloc(n * sizeof(double));
        result = (double *)malloc(n * sizeof(double));

        // Initialize matrix and vector
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                matrix[i * n + j] = rand() % 10; // Random values for demo
            }
            vector[i] = rand() % 10; // Random values for demo
        }
    }

    // Broadcast the order of the matrix to all processes
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int local_n = n / size; // Rows per process
    local_matrix = (double *)malloc(local_n * n * sizeof(double));
    local_result = (double *)malloc(local_n * sizeof(double));

    // Distribute matrix blocks to all processes
    MPI_Scatter(matrix, local_n * n, MPI_DOUBLE, local_matrix, local_n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Broadcast the vector to all processes
    MPI_Bcast(vector, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Local multiplication
    matrix_vector_multiply(local_matrix, vector, local_result, local_n, n);

    // Gather results from all processes
    MPI_Gather(local_result, local_n, MPI_DOUBLE, result, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Process 0 prints the result
    if (rank == 0)
    {
        printf("Result vector:\n");
        for (int i = 0; i < n; i++)
        {
            printf("%f ", result[i]);
        }
        printf("\n");

        // Free allocated memory
        free(matrix);
        free(vector);
        free(result);
    }

    // Free local buffers
    free(local_matrix);
    free(local_result);

    MPI_Finalize();
    return 0;
}
