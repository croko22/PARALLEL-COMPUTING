#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define NUM_THREADS 4
double a = 0.0, b = 1.0; // intervalo [a, b]
int n = 1000000;         // número de trapezoides
double sum = 0.0;
sem_t semaphore;

// función de ejemplo: f(x) = x^2
double f(double x)
{
    return x * x;
}

void get_data()
{
    printf("Ingrese el valor de a: ");
    scanf("%lf", &a);
    printf("Ingrese el valor de b: ");
    scanf("%lf", &b);
    printf("Ingrese el número de trapezoides: ");
    scanf("%d", &n);
}

void *trapezoid(void *rank)
{
    int my_rank = (long)rank;
    int local_n = n / NUM_THREADS;
    double h = (b - a) / n;
    double local_a = a + my_rank * local_n * h;
    double local_b = local_a + local_n * h;
    double local_sum = 0.0;
    for (int i = 0; i < local_n; i++)
    {
        double x = local_a + i * h;
        local_sum += f(x) + f(x + h);
    }
    local_sum = local_sum * h / 2.0;

    // Sección 3: Semáforo
    sem_wait(&semaphore);
    sum += local_sum;
    sem_post(&semaphore);

    return NULL;
}

int main()
{
    pthread_t threads[NUM_THREADS];
    sem_init(&semaphore, 0, 1);

    get_data();

    for (long thread = 0; thread < NUM_THREADS; thread++)
        pthread_create(&threads[thread], NULL, trapezoid, (void *)thread);
    for (long thread = 0; thread < NUM_THREADS; thread++)
        pthread_join(threads[thread], NULL);
    printf("Integral estimada: %f\n", sum);
    sem_destroy(&semaphore);
    return 0;
}