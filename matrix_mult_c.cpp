#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

void classic_matrix_multiplication(const vector<vector<double>> &A, const vector<vector<double>> &B, vector<vector<double>> &C, int N)
{
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
    int sizes[] = {100, 500, 1000}; // Diferentes tamaños de matriz a evaluar

    for (int N : sizes)
    {
        // Inicializar las matrices A, B, C
        vector<vector<double>> A(N, vector<double>(N, 1.0));
        vector<vector<double>> B(N, vector<double>(N, 1.0));
        vector<vector<double>> C(N, vector<double>(N, 0.0));

        // Medir el tiempo de ejecución de la multiplicación clásica
        auto start = chrono::high_resolution_clock::now();
        classic_matrix_multiplication(A, B, C, N);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = end - start;

        cout << "Tamaño de matriz: " << N << "x" << N << endl;
        cout << "Tiempo de multiplicación clásica: " << duration.count() << " segundos" << endl;
    }

    return 0;
}
