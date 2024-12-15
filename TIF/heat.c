#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void heat_diffusion(double *u, double *u_next, int N, int iterations, double alpha)
{
    for (int iter = 0; iter < iterations; iter++)
    {
#pragma omp parallel for
        for (int i = 1; i < N - 1; i++)
        {
            u_next[i] = u[i] + alpha * (u[i - 1] - 2 * u[i] + u[i + 1]);
        }
#pragma omp parallel for
        for (int i = 1; i < N - 1; i++)
        {
            u[i] = u_next[i];
        }
    }
}

int main()
{
    FILE *file = fopen("heat_results_dense.csv", "w");
    if (file == NULL)
    {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    fprintf(file, "N,iterations,execution_time\n");

    double alpha = 0.01;

    // Valores de N en escala logarítmica
    int N_values[] = {1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 500000};

    // Valores de iteraciones
    int iterations_values[] = {100, 200, 300, 500, 1000, 2000, 5000};

    for (int i = 0; i < sizeof(N_values) / sizeof(N_values[0]); i++)
    {
        int N = N_values[i];
        for (int j = 0; j < sizeof(iterations_values) / sizeof(iterations_values[0]); j++)
        {
            int iterations = iterations_values[j];

            double *u = malloc(N * sizeof(double));
            double *u_next = malloc(N * sizeof(double));

            for (int k = 0; k < N; k++)
            {
                u[k] = (k == 0 || k == N - 1) ? 100.0 : 0.0; // Condiciones iniciales
            }

            double start = omp_get_wtime();
            heat_diffusion(u, u_next, N, iterations, alpha);
            double end = omp_get_wtime();

            fprintf(file, "%d,%d,%.6f\n", N, iterations, end - start);

            free(u);
            free(u_next);
        }
    }

    fclose(file);
    return 0;
}