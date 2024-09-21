#include <stdio.h>
#include <time.h>

#define MAX 1000

double A[MAX][MAX], x[MAX], y[MAX];

// Función para inicializar la matriz A y el vector x
void initialize_arrays()
{
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
        {
            A[i][j] = (double)(i + j);
        }
        x[i] = (double)i;
        y[i] = 0.0;
    }
}

// Función para reinicializar y a 0
void reset_y()
{
    for (int i = 0; i < MAX; i++)
    {
        y[i] = 0.0;
    }
}

int main()
{
    clock_t start, end;
    double time_taken;

    // Inicializar A y x
    initialize_arrays();

    // Medir el tiempo del primer par de bucles (i-j)
    reset_y(); // Reinicializar y a 0
    start = clock();

    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
        {
            y[i] += A[i][j] * x[j];
        }
    }

    end = clock();
    time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Tiempo primer bucle (i-j): %f segundos\n", time_taken);

    // Medir el tiempo del segundo par de bucles (j-i)
    reset_y(); // Reinicializar y a 0
    start = clock();

    for (int j = 0; j < MAX; j++)
    {
        for (int i = 0; i < MAX; i++)
        {
            y[i] += A[i][j] * x[j];
        }
    }

    end = clock();
    time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Tiempo segundo bucle (j-i): %f segundos\n", time_taken);

    return 0;
}
