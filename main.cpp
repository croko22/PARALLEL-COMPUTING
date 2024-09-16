#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

const int MAX = 1000;

int main(int argc, char const *argv[])
{
    double A[MAX][MAX], x[MAX], y[MAX];

    // Primer par de bucles
    auto start1 = chrono::high_resolution_clock::now();
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
        {
            y[i] += A[i][j] * x[j];
        }
    }
    auto end1 = chrono::high_resolution_clock::now();
    chrono::duration<double> duration1 = end1 - start1;

    // Reinicializar y
    fill(begin(y), end(y), 0);

    // Segundo par de bucles
    auto start2 = chrono::high_resolution_clock::now();
    for (int j = 0; j < MAX; j++)
    {
        for (int i = 0; i < MAX; i++)
        {
            y[i] += A[i][j] * x[j];
        }
    }
    auto end2 = chrono::high_resolution_clock::now();
    chrono::duration<double> duration2 = end2 - start2;

    // Output the durations
    cout << "First pair of loops duration: " << duration1.count() << " seconds" << endl;
    cout << "Second pair of loops duration: " << duration2.count() << " seconds" << endl;

    return 0;
}