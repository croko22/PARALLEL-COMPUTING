#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip> // Para formatear la tabla

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

double measure_time(void (*matrix_multiplication)(const vector<vector<double>> &, const vector<vector<double>> &, vector<vector<double>> &, int), const vector<vector<double>> &A, const vector<vector<double>> &B, vector<vector<double>> &C, int N)
{
    auto start = chrono::high_resolution_clock::now();
    matrix_multiplication(A, B, C, N);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    return duration.count();
}

double measure_blocked_time(void (*matrix_multiplication)(const vector<vector<double>> &, const vector<vector<double>> &, vector<vector<double>> &, int, int), const vector<vector<double>> &A, const vector<vector<double>> &B, vector<vector<double>> &C, int N, int block_size)
{
    auto start = chrono::high_resolution_clock::now();
    matrix_multiplication(A, B, C, N, block_size);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    return duration.count();
}

void print_comparison_table(const vector<int> &sizes, const vector<double> &classic_times, const vector<double> &blocked_times)
{
    cout << setw(10) << "Tamaño" << setw(25) << "Multiplicación Clásica (s)" << setw(25) << "Multiplicación por Bloques (s)" << endl;
    cout << string(60, '-') << endl;
    for (size_t i = 0; i < sizes.size(); i++)
    {
        cout << setw(10) << sizes[i] << setw(25) << classic_times[i] << setw(25) << blocked_times[i] << endl;
    }
}

int main()
{
    vector<int> sizes = {100, 500, 1000}; // Diferentes tamaños de matriz a evaluar
    int block_size = 50;                  // Tamaño del bloque

    vector<double> classic_times;
    vector<double> blocked_times;

    for (int N : sizes)
    {
        // Inicializar las matrices A, B, C
        vector<vector<double>> A(N, vector<double>(N, 1.0));
        vector<vector<double>> B(N, vector<double>(N, 1.0));
        vector<vector<double>> C(N, vector<double>(N, 0.0));

        // Medir el tiempo de ejecución de la multiplicación clásica
        double classic_time = measure_time(classic_matrix_multiplication, A, B, C, N);
        classic_times.push_back(classic_time);

        // Reinicializar la matriz C
        fill(C.begin(), C.end(), vector<double>(N, 0.0));

        // Medir el tiempo de ejecución de la multiplicación por bloques
        double blocked_time = measure_blocked_time(blocked_matrix_multiplication, A, B, C, N, block_size);
        blocked_times.push_back(blocked_time);
    }

    // Mostrar la comparación en una tabla
    print_comparison_table(sizes, classic_times, blocked_times);

    return 0;
}
