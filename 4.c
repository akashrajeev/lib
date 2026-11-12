
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t fork_sem[5], room;

void *philosopher(void *arg) {
    int i = *(int *)arg;

    sem_wait(&room);

    sem_wait(&fork_sem[i]);
    sem_wait(&fork_sem[(i+1)%5]);

    printf("Philosopher %d is eating\n", i+1);
    sleep(1);

    sem_post(&fork_sem[i]);
    sem_post(&fork_sem[(i+1)%5]);

    sem_post(&room);
    return NULL;
}

int main() {
    pthread_t t[5];
    int id[5];

    sem_init(&room, 0, 4);

    for(int i=0; i<5; i++)
        sem_init(&fork_sem[i], 0, 1);

    for(int i=0; i<5; i++) {
        id[i] = i;
        pthread_create(&t[i], NULL, philosopher, &id[i]);
    }

    for(int i=0; i<5; i++)
        pthread_join(t[i], NULL);

    for(int i=0; i<5; i++)
        sem_destroy(&fork_sem[i]);

    sem_destroy(&room);
    return 0;
}
