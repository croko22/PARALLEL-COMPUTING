#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// Función para verificar errores
void Check_for_error(int local_ok, char fname[], char message[], MPI_Comm comm);

// Función para obtener las dimensiones de la matriz y el vector
void Get_dims(int *m_p, int *local_m_p, int *n_p, int *local_n_p, int my_rank, int comm_sz, MPI_Comm comm);

// Función para asignar memoria para los arreglos locales
void Allocate_arrays(double **local_A_pp, double **local_x_pp, double **local_y_pp, int local_m, int n, int local_n, MPI_Comm comm);

// Función para leer la matriz
void Read_matrix(char prompt[], double local_A[], int m, int local_m, int n, int my_rank, MPI_Comm comm);

// Función para leer el vector
void Read_vector(char prompt[], double local_vec[], int n, int local_n, int my_rank, MPI_Comm comm);

// Función para imprimir la matriz
void Print_matrix(char title[], double local_A[], int m, int local_m, int n, int my_rank, MPI_Comm comm);

// Función para imprimir el vector
void Print_vector(char title[], double local_vec[], int n, int local_n, int my_rank, MPI_Comm comm);

// Función para realizar la multiplicación matriz-vector
void Mat_vect_mult(double local_A[], double local_x[], double local_y[], int local_m, int n, int local_n, MPI_Comm comm);

int main(void)
{
    double *local_A;
    double *local_x;
    double *local_y;
    int m, local_m, n, local_n;
    int my_rank, comm_sz;
    MPI_Comm comm;

    MPI_Init(NULL, NULL); // Inicializar MPI
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &comm_sz); // Obtener el tamaño del comunicador
    MPI_Comm_rank(comm, &my_rank); // Obtener el rango del proceso

    Get_dims(&m, &local_m, &n, &local_n, my_rank, comm_sz, comm);             // Obtener dimensiones
    Allocate_arrays(&local_A, &local_x, &local_y, local_m, n, local_n, comm); // Asignar memoria
    Read_matrix("A", local_A, m, local_m, n, my_rank, comm);                  // Leer matriz
#ifdef DEBUG
    Print_matrix("A", local_A, m, local_m, n, my_rank, comm); // Imprimir matriz si DEBUG está definido
#endif
    Read_vector("x", local_x, n, local_n, my_rank, comm); // Leer vector
#ifdef DEBUG
    Print_vector("x", local_x, n, local_n, my_rank, comm); // Imprimir vector si DEBUG está definido
#endif

    Mat_vect_mult(local_A, local_x, local_y, local_m, n, local_n, comm); // Multiplicación matriz-vector

    Print_vector("y", local_y, m, local_m, my_rank, comm); // Imprimir resultado

    free(local_A); // Liberar memoria
    free(local_x);
    free(local_y);
    MPI_Finalize(); // Finalizar MPI
    return 0;
} /* main */

// Función para verificar errores
void Check_for_error(int local_ok, char fname[], char message[], MPI_Comm comm)
{
    int ok;

    MPI_Allreduce(&local_ok, &ok, 1, MPI_INT, MPI_MIN, comm);
    if (ok == 0)
    {
        int my_rank;
        MPI_Comm_rank(comm, &my_rank);
        if (my_rank == 0)
        {
            fprintf(stderr, "Proc %d > En %s, %s\n", my_rank, fname, message);
            fflush(stderr);
        }
        MPI_Finalize();
        exit(-1);
    }
} /* Check_for_error */

// Función para obtener las dimensiones de la matriz y el vector
void Get_dims(int *m_p, int *local_m_p, int *n_p, int *local_n_p, int my_rank, int comm_sz, MPI_Comm comm)
{
    int local_ok = 1;

    if (my_rank == 0)
    {
        printf("Ingrese el número de filas\n");
        scanf("%d", m_p);
        printf("Ingrese el número de columnas\n");
        scanf("%d", n_p);
    }
    MPI_Bcast(m_p, 1, MPI_INT, 0, comm);
    MPI_Bcast(n_p, 1, MPI_INT, 0, comm);
    if (*m_p <= 0 || *n_p <= 0 || *m_p % comm_sz != 0 || *n_p % comm_sz != 0)
        local_ok = 0;
    Check_for_error(local_ok, "Get_dims", "m y n deben ser positivos y divisibles por comm_sz", comm);

    *local_m_p = *m_p / comm_sz;
    *local_n_p = *n_p / comm_sz;
} /* Get_dims */

// Función para asignar memoria para los arreglos locales
void Allocate_arrays(double **local_A_pp, double **local_x_pp, double **local_y_pp, int local_m, int n, int local_n, MPI_Comm comm)
{
    int local_ok = 1;

    *local_A_pp = malloc(local_m * n * sizeof(double));
    *local_x_pp = malloc(local_n * sizeof(double));
    *local_y_pp = malloc(local_m * sizeof(double));

    if (*local_A_pp == NULL || local_x_pp == NULL || local_y_pp == NULL)
        local_ok = 0;
    Check_for_error(local_ok, "Allocate_arrays", "No se pueden asignar los arreglos locales", comm);
} /* Allocate_arrays */

// Función para leer la matriz
void Read_matrix(char prompt[], double local_A[], int m, int local_m, int n, int my_rank, MPI_Comm comm)
{
    double *A = NULL;
    int local_ok = 1;
    int i, j;

    if (my_rank == 0)
    {
        A = malloc(m * n * sizeof(double));
        if (A == NULL)
            local_ok = 0;
        Check_for_error(local_ok, "Read_matrix", "No se puede asignar la matriz temporal", comm);
        printf("Ingrese la matriz %s\n", prompt);
        for (i = 0; i < m; i++)
            for (j = 0; j < n; j++)
                scanf("%lf", &A[i * n + j]);
        MPI_Scatter(A, local_m * n, MPI_DOUBLE, local_A, local_m * n, MPI_DOUBLE, 0, comm);
        free(A);
    }
    else
    {
        Check_for_error(local_ok, "Read_matrix", "No se puede asignar la matriz temporal", comm);
        MPI_Scatter(A, local_m * n, MPI_DOUBLE, local_A, local_m * n, MPI_DOUBLE, 0, comm);
    }
} /* Read_matrix */

// Función para leer el vector
void Read_vector(char prompt[], double local_vec[], int n, int local_n, int my_rank, MPI_Comm comm)
{
    double *vec = NULL;
    int i, local_ok = 1;

    if (my_rank == 0)
    {
        vec = malloc(n * sizeof(double));
        if (vec == NULL)
            local_ok = 0;
        Check_for_error(local_ok, "Read_vector", "No se puede asignar el vector temporal", comm);
        printf("Ingrese el vector %s\n", prompt);
        for (i = 0; i < n; i++)
            scanf("%lf", &vec[i]);
        MPI_Scatter(vec, local_n, MPI_DOUBLE, local_vec, local_n, MPI_DOUBLE, 0, comm);
        free(vec);
    }
    else
    {
        Check_for_error(local_ok, "Read_vector", "No se puede asignar el vector temporal", comm);
        MPI_Scatter(vec, local_n, MPI_DOUBLE, local_vec, local_n, MPI_DOUBLE, 0, comm);
    }
} /* Read_vector */

// Función para imprimir la matriz
void Print_matrix(char title[], double local_A[], int m, int local_m, int n, int my_rank, MPI_Comm comm)
{
    double *A = NULL;
    int i, j, local_ok = 1;

    if (my_rank == 0)
    {
        A = malloc(m * n * sizeof(double));
        if (A == NULL)
            local_ok = 0;
        Check_for_error(local_ok, "Print_matrix", "No se puede asignar la matriz temporal", comm);
        MPI_Gather(local_A, local_m * n, MPI_DOUBLE, A, local_m * n, MPI_DOUBLE, 0, comm);
        printf("\nLa matriz %s\n", title);
        for (i = 0; i < m; i++)
        {
            for (j = 0; j < n; j++)
                printf("%f ", A[i * n + j]);
            printf("\n");
        }
        printf("\n");
        free(A);
    }
    else
    {
        Check_for_error(local_ok, "Print_matrix", "No se puede asignar la matriz temporal", comm);
        MPI_Gather(local_A, local_m * n, MPI_DOUBLE, A, local_m * n, MPI_DOUBLE, 0, comm);
    }
} /* Print_matrix */

// Función para imprimir el vector
void Print_vector(char title[], double local_vec[], int n, int local_n, int my_rank, MPI_Comm comm)
{
    double *vec = NULL;
    int i, local_ok = 1;

    if (my_rank == 0)
    {
        vec = malloc(n * sizeof(double));
        if (vec == NULL)
            local_ok = 0;
        Check_for_error(local_ok, "Print_vector", "No se puede asignar el vector temporal", comm);
        MPI_Gather(local_vec, local_n, MPI_DOUBLE, vec, local_n, MPI_DOUBLE, 0, comm);
        printf("\nEl vector %s\n", title);
        for (i = 0; i < n; i++)
            printf("%f ", vec[i]);
        printf("\n");
        free(vec);
    }
    else
    {
        Check_for_error(local_ok, "Print_vector", "No se puede asignar el vector temporal", comm);
        MPI_Gather(local_vec, local_n, MPI_DOUBLE, vec, local_n, MPI_DOUBLE, 0, comm);
    }
} /* Print_vector */

// Función para realizar la multiplicación matriz-vector
void Mat_vect_mult(double local_A[], double local_x[], double local_y[], int local_m, int n, int local_n, MPI_Comm comm)
{
    double *x;
    int local_i, j;
    int local_ok = 1;

    x = malloc(n * sizeof(double));
    if (x == NULL)
        local_ok = 0;
    Check_for_error(local_ok, "Mat_vect_mult", "No se puede asignar el vector temporal", comm);
    MPI_Allgather(local_x, local_n, MPI_DOUBLE, x, local_n, MPI_DOUBLE, comm);

    for (local_i = 0; local_i < local_m; local_i++)
    {
        local_y[local_i] = 0.0;
        for (j = 0; j < n; j++)
            local_y[local_i] += local_A[local_i * n + j] * x[j];
    }
    free(x);
} /* Mat_vect_mult */