#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000
#define NUM_RUNS 100

int main(int argc, char const *argv[])
{
    double A[MAX][MAX], x[MAX], y[MAX];
    clock_t start, end;
    double total_duration1 = 0, total_duration2 = 0;

    // Inicializar A y x, asignar y = 0
    for (int i = 0; i < MAX; i++)
    {
        x[i] = i * 0.01;
        for (int j = 0; j < MAX; j++)
        {
            A[i][j] = (i + j) * 0.01;
        }
    }

    for (int run = 0; run < NUM_RUNS; run++)
    {
        // Reinicializar y
        for (int i = 0; i < MAX; i++)
        {
            y[i] = 0;
        }

        // Primer par de bucles
        start = clock();
        for (int i = 0; i < MAX; i++)
        {
            for (int j = 0; j < MAX; j++)
            {
                y[i] += A[i][j] * x[j];
            }
        }
        end = clock();
        total_duration1 += ((double)(end - start)) / CLOCKS_PER_SEC;

        // Reinicializar y
        for (int i = 0; i < MAX; i++)
        {
            y[i] = 0;
        }

        // Segundo par de bucles
        start = clock();
        for (int j = 0; j < MAX; j++)
        {
            for (int i = 0; i < MAX; i++)
            {
                y[i] += A[i][j] * x[j];
            }
        }
        end = clock();
        total_duration2 += ((double)(end - start)) / CLOCKS_PER_SEC;
    }

    double avg_duration1 = total_duration1 / NUM_RUNS;
    double avg_duration2 = total_duration2 / NUM_RUNS;

    // Imprimir los tiempos promedio de duración
    printf("Promedio de duración del primer par de bucles: %f segundos\n", avg_duration1);
    printf("Promedio de duración del segundo par de bucles: %f segundos\n", avg_duration2);

    return 0;
}