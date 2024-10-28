#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// Función para fusionar dos arreglos ordenados en uno solo
void merge(int *array1, int size1, int *array2, int size2, int *merged)
{
    int i = 0, j = 0, k = 0;
    while (i < size1 && j < size2)
    {
        if (array1[i] < array2[j])
        {
            merged[k++] = array1[i++];
        }
        else
        {
            merged[k++] = array2[j++];
        }
    }
    while (i < size1)
        merged[k++] = array1[i++];
    while (j < size2)
        merged[k++] = array2[j++];
}

// Función de ordenamiento por fusión
void merge_sort(int *array, int size)
{
    if (size < 2)
        return;
    int mid = size / 2;
    merge_sort(array, mid);
    merge_sort(array + mid, size - mid);
    int *temp = (int *)malloc(size * sizeof(int));
    merge(array, mid, array + mid, size - mid, temp);
    for (int i = 0; i < size; i++)
        array[i] = temp[i];
    free(temp);
}

int main(int argc, char **argv)
{
    int rank, comm_sz, n;
    MPI_Init(&argc, &argv);                  // Inicializar MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);    // Obtener el rango del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); // Obtener el tamaño del comunicador

    if (rank == 0)
    {
        printf("Enter the number of integers (n): ");
        scanf("%d", &n); // Leer el número de enteros
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD); // Difundir el valor de n a todos los procesos

    int local_n = n / comm_sz;
    int *local_array = (int *)malloc(local_n * sizeof(int));
    // Generar enteros aleatorios
    for (int i = 0; i < local_n; i++)
    {
        local_array[i] = rand() % 100; // Enteros aleatorios entre 0 y 99
    }

    // Ordenar el arreglo local
    merge_sort(local_array, local_n);

    // Recolectar las listas ordenadas en el proceso 0
    int *sorted_array = NULL;
    if (rank == 0)
    {
        sorted_array = (int *)malloc(n * sizeof(int));
    }
    MPI_Gather(local_array, local_n, MPI_INT, sorted_array, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    // Fusionar todos los arreglos ordenados en el proceso 0
    if (rank == 0)
    {
        merge_sort(sorted_array, n);
        printf("Sorted array: ");
        for (int i = 0; i < n; i++)
        {
            printf("%d ", sorted_array[i]);
        }
        printf("\n");
        free(sorted_array);
    }

    free(local_array);
    MPI_Finalize(); // Finalizar MPI
    return 0;
}