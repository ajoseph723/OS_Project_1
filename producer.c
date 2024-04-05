#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <fcntl.h>

#define BUFFER_SIZE 2

int buffer[BUFFER_SIZE];
sem_t *empty, *full, *mutex;

void *producer(void *arg)
{
    int item = 0;
    while (1)
    {
        usleep(rand() % 1000000); // Simulate production time
        sem_wait(empty);
        sem_wait(mutex);
        buffer[item++] = 1;
        printf("Produced item %d\n", item);
        sem_post(mutex);
        sem_post(full);
        if (item >= BUFFER_SIZE)
            item = 0;
    }
}

int main()
{
    empty = sem_open("/empty_sem", O_CREAT, 0666, BUFFER_SIZE);
    full = sem_open("/full_sem", O_CREAT, 0666, 0);
    mutex = sem_open("/mutex_sem", O_CREAT, 0666, 1);

    pthread_t producer_thread;
    pthread_create(&producer_thread, NULL, producer, NULL);

    pthread_join(producer_thread, NULL);

    sem_close(empty);
    sem_close(full);
    sem_close(mutex);
    sem_unlink("/empty_sem");
    sem_unlink("/full_sem");
    sem_unlink("/mutex_sem");

    return 0;
}