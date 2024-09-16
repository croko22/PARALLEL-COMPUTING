#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;

void blocked_matrix_multiplication(const vector<vector<double>> &A, const vector<vector<double>> &B, vector<vector<double>> &C, int N, int block_size)
{
    for (int ii = 0; ii < N; ii += block_size)
    {
        for (int jj = 0; jj < N; jj += block_size)
        {
            for (int kk = 0; kk < N; kk += block_size)
            {
                for (int i = ii; i < min(ii + block_size, N); i++)
                {
                    for (int j = jj; j < min(jj + block_size, N); j++)
                    {
                        for (int k = kk; k < min(kk + block_size, N); k++)
                        {
                            C[i][j] += A[i][k] * B[k][j];
                        }
                    }
                }
            }
        }
    }
}

int main()
{
    int sizes[] = {100, 500, 1000}; // Diferentes tamaños de matriz a evaluar
    int block_size = 50;            // Tamaño del bloque

    for (int N : sizes)
    {
        // Inicializar las matrices A, B, C
        vector<vector<double>> A(N, vector<double>(N, 1.0));
        vector<vector<double>> B(N, vector<double>(N, 1.0));
        vector<vector<double>> C(N, vector<double>(N, 0.0));

        // Medir el tiempo de ejecución de la multiplicación por bloques
        auto start = chrono::high_resolution_clock::now();
        blocked_matrix_multiplication(A, B, C, N, block_size);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = end - start;

        cout << "Tamaño de matriz: " << N << "x" << N << endl;
        cout << "Tiempo de multiplicación por bloques: " << duration.count() << " segundos" << endl;
    }

    return 0;
}
