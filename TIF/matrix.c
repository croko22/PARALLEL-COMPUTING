#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void matrix_multiply(int **A, int **B, int **C, int N)
{
#pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            C[i][j] = 0;
            for (int k = 0; k < N; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main()
{
    FILE *file = fopen("matrix_times.csv", "w"); // Archivo CSV para guardar los tiempos
    if (file == NULL)
    {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    fprintf(file, "N,Threads,Time\n"); // Encabezado del CSV

    int sizes[] = {256, 512, 1024, 2048, 4096};
    // , 2048, 4096, 6144, 8192, 10240, 12288
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int threads[] = {4, 8}; // Número de hilos a probar
    int num_threads = sizeof(threads) / sizeof(threads[0]);

    for (int t = 0; t < num_threads; t++)
    {
        printf("Threads = %d\n", threads[t]);
        omp_set_num_threads(threads[t]);
        for (int s = 0; s < num_sizes; s++)
        {
            int N = sizes[s];
            printf("N = %d\n", N);
            int **A = malloc(N * sizeof(int *));
            int **B = malloc(N * sizeof(int *));
            int **C = malloc(N * sizeof(int *));
            for (int i = 0; i < N; i++)
            {
                A[i] = malloc(N * sizeof(int));
                B[i] = malloc(N * sizeof(int));
                C[i] = malloc(N * sizeof(int));
                for (int j = 0; j < N; j++)
                {
                    A[i][j] = (rand() % 2001) - 1000; // Valores aleatorios entre -1000 y 1000
                    B[i][j] = (rand() % 2001) - 1000; // Valores aleatorios entre -1000 y 1000
                }
            }

            double start = omp_get_wtime();
            matrix_multiply(A, B, C, N);
            double end = omp_get_wtime();
            double time_taken = end - start;

            fprintf(file, "%d,%d,%f\n", N, threads[t], time_taken);

            for (int i = 0; i < N; i++)
            {
                free(A[i]);
                free(B[i]);
                free(C[i]);
            }
            free(A);
            free(B);
            free(C);
        }
    }

    fclose(file);
    return 0;
}