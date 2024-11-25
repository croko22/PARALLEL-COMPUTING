#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

int bin_count;
float min_meas, max_meas;
int data_count, thread_count;
float *data;
int *bin_counts;
float *bin_maxes;
pthread_mutex_t mutex;

void Usage(char prog_name[]);
void Get_args(char *argv[], int *bin_count_p, float *min_meas_p, float *max_meas_p, int *data_count_p, int *thread_count_p);
void Gen_data(float min_meas, float max_meas, float data[], int data_count);
void Gen_bins(float min_meas, float max_meas, int bin_count, float bin_maxes[]);
void *Thread_work(void *rank);
int Which_bin(float val, float bin_maxes[], int bin_count, float min_meas);

int main(int argc, char *argv[])
{
    long thread;
    pthread_t *thread_handles;

    if (argc != 6)
        Usage(argv[0]);
    Get_args(argv, &bin_count, &min_meas, &max_meas, &data_count, &thread_count);

    data = (float *)malloc(data_count * sizeof(float));
    bin_counts = (int *)malloc(bin_count * sizeof(int));
    bin_maxes = (float *)malloc(bin_count * sizeof(float));
    thread_handles = (pthread_t *)malloc(thread_count * sizeof(pthread_t));

    pthread_mutex_init(&mutex, NULL);

    Gen_data(min_meas, max_meas, data, data_count);
    Gen_bins(min_meas, max_meas, bin_count, bin_maxes);

    // Initialize bin_counts to 0
    for (int i = 0; i < bin_count; i++)
    {
        bin_counts[i] = 0;
    }

    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_create(&thread_handles[thread], NULL, Thread_work, (void *)thread);
    }

    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_join(thread_handles[thread], NULL);
    }

    printf("Histogram:\n");
    for (int i = 0; i < bin_count; i++)
    {
        printf("Bin %d: ", i);
        for (int j = 0; j < bin_counts[i]; j++)
        {
            printf("X");
        }
        printf("\n");
    }

    free(data);
    free(bin_counts);
    free(bin_maxes);
    free(thread_handles);
    pthread_mutex_destroy(&mutex);

    return 0;
}

void Usage(char prog_name[])
{
    fprintf(stderr, "Usage: %s <bin_count> <min_meas> <max_meas> <data_count> <thread_count>\n", prog_name);
    exit(0);
}

void Get_args(char *argv[], int *bin_count_p, float *min_meas_p, float *max_meas_p, int *data_count_p, int *thread_count_p)
{
    *bin_count_p = strtol(argv[1], NULL, 10);
    *min_meas_p = strtof(argv[2], NULL);
    *max_meas_p = strtof(argv[3], NULL);
    *data_count_p = strtol(argv[4], NULL, 10);
    *thread_count_p = strtol(argv[5], NULL, 10);
}

void Gen_data(float min_meas, float max_meas, float data[], int data_count)
{
    srand(0);
    for (int i = 0; i < data_count; i++)
    {
        data[i] = min_meas + (max_meas - min_meas) * rand() / ((double)RAND_MAX);
    }
    // Datos generados
    printf("Generated data:\n");
    for (int i = 0; i < data_count; i++)
    {
        printf("%f ", data[i]);
    }
    printf("\n");
}

void Gen_bins(float min_meas, float max_meas, int bin_count, float bin_maxes[])
{
    float bin_width = (max_meas - min_meas) / bin_count;
    for (int i = 0; i < bin_count; i++)
    {
        bin_maxes[i] = min_meas + (i + 1) * bin_width;
    }
    // Maximos de los bins
    printf("Bin maxes:\n");
    for (int i = 0; i < bin_count; i++)
    {
        printf("%f ", bin_maxes[i]);
    }
    printf("\n");
}

void *Thread_work(void *rank)
{
    long my_rank = (long)rank;
    int my_first_i = my_rank * data_count / thread_count;
    int my_last_i = (my_rank + 1) * data_count / thread_count;
    int my_bin;

    for (int i = my_first_i; i < my_last_i; i++)
    {
        my_bin = Which_bin(data[i], bin_maxes, bin_count, min_meas);
        pthread_mutex_lock(&mutex);
        bin_counts[my_bin]++;
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int Which_bin(float val, float bin_maxes[], int bin_count, float min_meas)
{
    int i;
    for (i = 0; i < bin_count; i++)
    {
        if (val < bin_maxes[i])
        {
            return i;
        }
    }
    return bin_count - 1;
}