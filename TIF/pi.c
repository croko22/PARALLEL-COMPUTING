#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

double estimate_pi(int num_points)
{
    int inside_circle = 0;
#pragma omp parallel for reduction(+ : inside_circle)
    for (int i = 0; i < num_points; i++)
    {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1.0)
        {
            inside_circle++;
        }
    }
    return 4.0 * inside_circle / num_points;
}

int main()
{
    FILE *file = fopen("pi.csv", "w");
    if (file == NULL)
    {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    fprintf(file, "num_points,threads,pi_estimate,execution_time,precision\n");

    int num_points_list[] = {1000, 10000, 100000, 1000000, 10000000};
    int num_experiments = sizeof(num_points_list) / sizeof(num_points_list[0]);
    int threads[] = {4, 8, 16};
    int num_threads = sizeof(threads) / sizeof(threads[0]);
    const double pi_real = 3.141592653589793;

    for (int t = 0; t < num_threads; t++)
    {
        omp_set_num_threads(threads[t]);
        for (int i = 0; i < num_experiments; i++)
        {
            int num_points = num_points_list[i];

            double start = omp_get_wtime();
            double pi = estimate_pi(num_points);
            double end = omp_get_wtime();
            double precision = fabs(pi - pi_real);

            fprintf(file, "%d,%d,%.6f,%.6f,%.6f\n", num_points, threads[t], pi, end - start, precision);
        }
    }

    fclose(file);
    return 0;
}