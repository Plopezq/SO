#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NR_PHILOSOPHERS 5

pthread_t philosophers[NR_PHILOSOPHERS];
pthread_mutex_t forks[NR_PHILOSOPHERS];


void init()
{
    int i;
    for(i=0; i<NR_PHILOSOPHERS; i++)
        pthread_mutex_init(&forks[i],NULL);
    
}

void think(int i) {
    printf("Philosopher %d thinking... \n" , i);
    sleep(random() % 10);
    printf("Philosopher %d stopped thinking!!! \n" , i);

}

void eat(int i) {
    printf("Philosopher %d eating... \n" , i);
    sleep(random() % 5);
    printf("Philosopher %d is not eating anymore!!! \n" , i);

}

void toSleep(int i) {
    printf("Philosopher %d sleeping... \n" , i);
    sleep(random() % 10);
    printf("Philosopher %d is awake!!! \n" , i);
    
}

void* philosopher(void* i)
{
    int nPhilosopher = (int)i;
    int right = nPhilosopher;
    int left = (nPhilosopher - 1 == -1) ? NR_PHILOSOPHERS - 1 : (nPhilosopher - 1);
    while(1)
    {
        
        think(nPhilosopher);//PIENSA
        
        /// TRY TO GRAB BOTH FORKS (right and left)
        // Tomar tenedor izquierzo
        pthread_mutex_lock(&forks[left]);
            printf("Filosofo %d toma el tenedor izquierdo %d \n", nPhilosopher, left);
        //Tomar tenedor derecho
        pthread_mutex_lock(&forks[right]);
            printf("Filosofo %d toma el tenedor derecho %d \n", nPhilosopher, right);

        eat(nPhilosopher); //COME
        
        // PUT FORKS BACK ON THE TABLE

        // Tomar tenedor izquierzo
        pthread_mutex_unlock(&forks[left]);
            printf("Filosofo %d deja el tenedor izquierdo %d \n", nPhilosopher, left);
        //Tomar tenedor derecho
        pthread_mutex_unlock(&forks[right]);
            printf("Filosofo %d deja el tenedor derecho %d \n", nPhilosopher, right);
        
        toSleep(nPhilosopher); //DUERME
   }

}

int main()
{
    init();
    unsigned long i;
    for(i=0; i<NR_PHILOSOPHERS; i++)
        pthread_create(&philosophers[i], NULL, philosopher, (void*)i);
    
    for(i=0; i<NR_PHILOSOPHERS; i++)
        pthread_join(philosophers[i],NULL);
    return 0;
} 
