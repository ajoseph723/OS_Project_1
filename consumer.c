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

void *consumer(void *arg)
{
    int item = 0;
    while (1)
    {
        usleep(rand() % 1000000); // Simulate consumption time
        sem_wait(full);
        sem_wait(mutex);
        buffer[item++] = 0;
        printf("Consumed item %d\n", item);
        sem_post(mutex);
        sem_post(empty);
        if (item >= BUFFER_SIZE)
            item = 0;
    }
}

int main()
{
    empty = sem_open("/empty_sem", O_CREAT, 0666, BUFFER_SIZE);
    full = sem_open("/full_sem", O_CREAT, 0666, 0);
    mutex = sem_open("/mutex_sem", O_CREAT, 0666, 1);

    pthread_t consumer_thread;
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(consumer_thread, NULL);

    sem_close(empty);
    sem_close(full);
    sem_close(mutex);
    sem_unlink("/empty_sem");
    sem_unlink("/full_sem");
    sem_unlink("/mutex_sem");

    return 0;
}
