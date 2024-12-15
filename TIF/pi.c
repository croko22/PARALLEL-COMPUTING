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
    FILE *file4 = fopen("pi_4_threads.csv", "w");
    FILE *file8 = fopen("pi_8_threads.csv", "w");
    FILE *file16 = fopen("pi_16_threads.csv", "w");
    if (file4 == NULL || file8 == NULL || file16 == NULL)
    {
        printf("Error al abrir los archivos.\n");
        return 1;
    }

    fprintf(file4, "num_points,threads,pi_estimate,execution_time,precision\n");
    fprintf(file8, "num_points,threads,pi_estimate,execution_time,precision\n");
    fprintf(file16, "num_points,threads,pi_estimate,execution_time,precision\n");

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

            if (threads[t] == 4)
            {
                fprintf(file4, "%d,%d,%.6f,%.6f,%.6f\n", num_points, threads[t], pi, end - start, precision);
            }
            else if (threads[t] == 8)
            {
                fprintf(file8, "%d,%d,%.6f,%.6f,%.6f\n", num_points, threads[t], pi, end - start, precision);
            }
            else if (threads[t] == 16)
            {
                fprintf(file16, "%d,%d,%.6f,%.6f,%.6f\n", num_points, threads[t], pi, end - start, precision);
            }
        }
    }

    fclose(file4);
    fclose(file8);
    fclose(file16);
    return 0;
}