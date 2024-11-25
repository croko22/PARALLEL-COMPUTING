# 4.1
Escribe un programa en Pthreads que implemente el programa de histograma del Capítulo 2.

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

# 4.5
Escribe un programa en Pthreads que implemente una "cola de tareas". El hilo principal comienza iniciando un número de hilos especificado por el usuario que inmediatamente se duermen en una espera de condición. El hilo principal genera bloques de tareas que deben ser llevadas a cabo por los otros hilos; cada vez que genera un nuevo bloque de tareas, despierta a un hilo con una señal de condición. Cuando un hilo termina de ejecutar su bloque de tareas, debe volver a una espera de condición. Cuando el hilo principal completa la generación de tareas, establece una variable global que indica que no habrá más tareas y despierta a todos los hilos con una transmisión de condición. Por el bien de la explicitud, haz que tus tareas sean operaciones de lista enlazada.