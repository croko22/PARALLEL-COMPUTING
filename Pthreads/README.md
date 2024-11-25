# 4.1
Escribe un programa en Pthreads que implemente el programa de histograma del Capítulo 2.

## Función `Thread_work`

La función `Thread_work` es la que ejecutan los hilos creados. Aquí se realiza el trabajo principal de cada hilo, que incluye:

1. **Cálculo del rango de datos**: Cada hilo procesa una porción de los datos.
2. **Acceso a recursos compartidos**: Los hilos utilizan el mutex para asegurar que solo un hilo accede a los contadores de bins a la vez, evitando condiciones de carrera.

### Ejemplo de la Función `Thread_work`

```c
void *Thread_work(void *rank) {
    long my_rank = (long)rank;
    int my_first_i = my_rank * data_count / thread_count;
    int my_last_i = (my_rank + 1) * data_count / thread_count;
    int my_bin;

    for (int i = my_first_i; i < my_last_i; i++) {
        my_bin = Which_bin(data[i], bin_maxes, bin_count, min_meas);
        pthread_mutex_lock(&mutex);
        bin_counts[my_bin]++;
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}
```

- **my_rank**: Identificador del hilo.
- **my_first_i y my_last_i**: Determinan el rango de datos que procesará el hilo.
- **pthread_mutex_lock y pthread_mutex_unlock**: Aseguran que solo un hilo accede a los contadores de bins a la vez.

### Conclusión

El uso de Pthreads en este código permite la ejecución concurrente de múltiples hilos para procesar datos en paralelo. El mutex asegura la exclusión mutua cuando los hilos acceden a recursos compartidos, evitando condiciones de carrera y asegurando la integridad de los datos.

## Compilación
```sh
gcc -o histogram histogram.c -lpthread
```

## Ejecución
```sh
./histogram 5 0.0 10.0 20 2
./histogram 10 0.0 100.0 1000 4
```

# 4.3
Escribe un programa en Pthreads que implemente la regla trapezoidal. Usa una variable compartida para la suma de los cálculos de todos los hilos. Usa espera activa, mutexes y semáforos para imponer la exclusión mutua en la sección crítica. ¿Qué ventajas y desventajas ves con cada enfoque?

### Ventajas y Desventajas de Cada Enfoque

#### Espera Activa

**Ventajas:**
- **Simplicidad:** Fácil de implementar.
- **Sin Bloqueos:** No requiere mecanismos adicionales.

**Desventajas:**
- **Consumo de CPU:** Usa muchos recursos de CPU.
- **Ineficiencia:** Ineficiente en términos de energía y rendimiento.

#### Mutexes

**Ventajas:**
- **Exclusión Mutua:** Asegura que solo un hilo accede a la vez.
- **Eficiencia:** Hilos bloqueados liberan la CPU.

**Desventajas:**
- **Complejidad:** Más difícil de implementar.
- **Riesgo de Deadlocks:** Posibilidad de bloqueos mutuos.

#### Semáforos

**Ventajas:**
- **Flexibilidad:** Sincroniza múltiples hilos.
- **Control Preciso:** Mejor control sobre recursos compartidos.

**Desventajas:**
- **Complejidad:** Difícil de manejar correctamente.
- **Sobrecarga:** Introduce sobrecarga adicional.

Cada método tiene sus pros y contras. La elección depende del contexto y los requisitos del sistema.

# 4.5
Escribe un programa en Pthreads que implemente una "cola de tareas". El hilo principal comienza iniciando un número de hilos especificado por el usuario que inmediatamente se duermen en una espera de condición. El hilo principal genera bloques de tareas que deben ser llevadas a cabo por los otros hilos; cada vez que genera un nuevo bloque de tareas, despierta a un hilo con una señal de condición. Cuando un hilo termina de ejecutar su bloque de tareas, debe volver a una espera de condición. Cuando el hilo principal completa la generación de tareas, establece una variable global que indica que no habrá más tareas y despierta a todos los hilos con una transmisión de condición. Por el bien de la explicitud, haz que tus tareas sean operaciones de lista enlazada.

### Inicialización de Pthreads

```c
pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;
TaskQueue queue = {NULL, NULL, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, false};
```

### Función `add_to_list`

```c
void add_to_list(int value) {
    pthread_mutex_lock(&list_mutex);
    Task *new_node = (Task *)malloc(sizeof(Task));
    new_node->value = value;
    new_node->next = linked_list;
    linked_list = new_node;
    pthread_mutex_unlock(&list_mutex);
}
```

### Función `submit_task`

```c
void submit_task(int value) {
    Task *new_task = (Task *)malloc(sizeof(Task));
    new_task->value = value;
    new_task->next = NULL;

    pthread_mutex_lock(&queue.mutex);
    if (queue.tail) {
        queue.tail->next = new_task;
    } else {
        queue.head = new_task;
    }
    queue.tail = new_task;
    pthread_cond_signal(&queue.cond);
    pthread_mutex_unlock(&queue.mutex);
}
```

### Función `Thread_work`

```c
void *Thread_work(void *rank) {
    while (true) {
        pthread_mutex_lock(&queue.mutex);
        while (queue.head == NULL && !queue.no_more_tasks) {
            pthread_cond_wait(&queue.cond, &queue.mutex);
        }
        if (queue.head == NULL && queue.no_more_tasks) {
            pthread_mutex_unlock(&queue.mutex);
            break;
        }

        Task *task = queue.head;
        queue.head = task->next;
        if (!queue.head) {
            queue.tail = NULL;
        }
        pthread_mutex_unlock(&queue.mutex);

        printf("Thread %ld processing task with value %d\n", pthread_self(), task->value);
        add_to_list(task->value);
        free(task);
    }

    return NULL;
}
```

### Función `main`

```c
int main(int argc, char *argv[]) {
    int thread_count = strtol(argv[1], NULL, 10);
    pthread_t *thread_handles = malloc(thread_count * sizeof(pthread_t));

    for (long thread = 0; thread < thread_count; thread++) {
        pthread_create(&thread_handles[thread], NULL, Thread_work, (void *)thread);
    }

    // Tareas
    for (int i = 0; i < 10; i++) {
        submit_task(i);
    }

    pthread_mutex_lock(&queue.mutex);
    queue.no_more_tasks = true;
    pthread_cond_broadcast(&queue.cond);
    pthread_mutex_unlock(&queue.mutex);

    for (long thread = 0; thread < thread_count; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }

    pthread_mutex_destroy(&queue.mutex);
    pthread_cond_destroy(&queue.cond);
    pthread_mutex_destroy(&list_mutex);

    return 0;
}
```

- **pthread_mutex_lock y pthread_mutex_unlock**: Aseguran la exclusión mutua cuando los hilos acceden a recursos compartidos.
- **pthread_cond_wait y pthread_cond_signal/broadcast**: Sincronizan el acceso a la cola de tareas, haciendo que los hilos esperen hasta que haya tareas disponibles o se indique que no habrá más tareas.
- **pthread_create**: Crea los hilos que ejecutarán la función `Thread_work`.
- **pthread_join**: Espera a que todos los hilos terminen su ejecución.
- **pthread_mutex_destroy y pthread_cond_destroy**: Destruyen los mutexes y la variable de condición, liberando los recursos asociados.