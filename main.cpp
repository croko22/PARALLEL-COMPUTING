#include <iostream>
#include <vector>
#include <algorithm>
#include <mpi.h>

using namespace std;

const int MAX = 1000; // Reducir el tamaño de las matrices
const int NUM_RUNS = 1000;

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    double A[MAX][MAX], x[MAX], y[MAX];
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
        fill(begin(y), end(y), 0);

        // Primer par de bucles
        double start1 = MPI_Wtime();
        for (int i = 0; i < MAX; i++)
        {
            for (int j = 0; j < MAX; j++)
            {
                y[i] += A[i][j] * x[j];
            }
        }
        double end1 = MPI_Wtime();
        total_duration1 += (end1 - start1);

        // Reinicializar y
        fill(begin(y), end(y), 0);

        // Segundo par de bucles
        double start2 = MPI_Wtime();
        for (int j = 0; j < MAX; j++)
        {
            for (int i = 0; i < MAX; i++)
            {
                y[i] += A[i][j] * x[j];
            }
        }
        double end2 = MPI_Wtime();
        total_duration2 += (end2 - start2);
    }

    double avg_duration1 = total_duration1 / NUM_RUNS;
    double avg_duration2 = total_duration2 / NUM_RUNS;

    // Imprimir los tiempos promedio de duración
    cout << "Promedio de duración del primer par de bucles: " << avg_duration1 << " segundos" << endl;
    cout << "Promedio de duración del segundo par de bucles: " << avg_duration2 << " segundos" << endl;

    MPI_Finalize();
    return 0;
}