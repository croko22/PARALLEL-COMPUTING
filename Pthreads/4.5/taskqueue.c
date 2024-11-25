#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

typedef struct Task
{
    int value;
    struct Task *next;
} Task;

typedef struct TaskQueue
{
    Task *head;
    Task *tail;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    bool no_more_tasks;
} TaskQueue;

TaskQueue queue;
pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;
Task *linked_list = NULL;

void add_to_list(int value)
{
    pthread_mutex_lock(&list_mutex);
    Task *new_node = (Task *)malloc(sizeof(Task));
    new_node->value = value;
    new_node->next = linked_list;
    linked_list = new_node;
    pthread_mutex_unlock(&list_mutex);
}

void *worker_thread(void *arg)
{
    while (true)
    {
        pthread_mutex_lock(&queue.mutex);
        while (!queue.head && !queue.no_more_tasks)
        {
            pthread_cond_wait(&queue.cond, &queue.mutex);
        }

        if (queue.no_more_tasks && !queue.head)
        {
            pthread_mutex_unlock(&queue.mutex);
            break;
        }

        Task *task = queue.head;
        queue.head = task->next;
        if (!queue.head)
            queue.tail = NULL;

        pthread_mutex_unlock(&queue.mutex);

        printf("Thread %ld processing task with value %d\n", pthread_self(), task->value);
        add_to_list(task->value);
        free(task);
    }

    printf("Thread %ld exiting.\n", pthread_self());
    return NULL;
}

void submit_task(int value)
{
    Task *new_task = (Task *)malloc(sizeof(Task));
    new_task->value = value;
    new_task->next = NULL;

    pthread_mutex_lock(&queue.mutex);
    if (queue.tail)
    {
        queue.tail->next = new_task;
    }
    else
    {
        queue.head = new_task;
    }
    queue.tail = new_task;

    pthread_cond_signal(&queue.cond);
    pthread_mutex_unlock(&queue.mutex);
}

int main()
{
    int num_threads = 4;
    pthread_t threads[num_threads];

    pthread_mutex_init(&queue.mutex, NULL);
    pthread_cond_init(&queue.cond, NULL);
    queue.head = queue.tail = NULL;
    queue.no_more_tasks = false;

    for (int i = 0; i < num_threads; i++)
    {
        pthread_create(&threads[i], NULL, worker_thread, NULL);
    }

    for (int i = 1; i <= 10; i++)
    {
        submit_task(i);

        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 50000 * 1000; // Pausa de 50 ms
        nanosleep(&ts, NULL);
    }

    pthread_mutex_lock(&queue.mutex);
    queue.no_more_tasks = true;
    pthread_cond_broadcast(&queue.cond);
    pthread_mutex_unlock(&queue.mutex);

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&queue.mutex);
    pthread_cond_destroy(&queue.cond);

    printf("Final linked list values:\n");
    Task *current = linked_list;
    while (current)
    {
        printf("%d ", current->value);
        Task *temp = current;
        current = current->next;
        free(temp);
    }
    printf("\n");

    return 0;
}
